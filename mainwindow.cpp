#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>

#include <glm/glm.hpp>
#include <QThread>
#include <QElapsedTimer>

#define PLAYER_SIZE 10

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    findLevels();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_level)
        delete m_level;
}

void MainWindow::findLevels()
{
    // TODO: VALIDATE CONTENTS OF THE TXT
    DIR* directory = opendir("./Levels");
    if(directory == NULL)
    {
        ui->textEditLog->appendPlainText("Can't open folder containing levels.\n");
        return;
    }

    struct dirent* item;
    while((item = readdir(directory)))
    {
        QString levelName = item->d_name;
        if(levelName.contains(".txt", Qt::CaseSensitive))
            ui->comboBoxLevel->addItem(levelName);
    }
    ui->textEditLog->appendPlainText("Level folder loaded.\n");
    closedir(directory);
}

int MainWindow::createSocket()
{
    /* create a socket */
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
       ui->textEditLog->appendPlainText("Can't create a socket.\n");
       //fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
       exit(1);
    }
    ui->textEditLog->appendPlainText("Created UDP socket.\n");
    return sock;
}
void MainWindow::configSocket(int sock)
{
    // Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already
    int enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
    // Set socket to nonblock mode, --recvfrom won't wait
    fcntl(sock, F_SETFL, O_NONBLOCK,1);
}

void MainWindow::startLoop()
{
#define SERVER_PORT 1234
#define SERVER_PORT2 1235

    int nBind;
    sockaddr_in stAddr, stAddr2;

    m_mainSocket = createSocket();
    configSocket(m_mainSocket);
    int secondarySocket = createSocket();
    configSocket(secondarySocket);

    /* SOCKET FOR PLAYER 1 */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    /* SOCKET FOR PLAYER 2 */
    memset(&stAddr2, 0, sizeof(struct sockaddr));
    stAddr2.sin_family = AF_INET;
    stAddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr2.sin_port = htons(SERVER_PORT2);

    nBind = bind(m_mainSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
       ui->textEditLog->appendPlainText(QString("Can't bind a name to a socket.\n"));
       exit(1);
    }

    nBind = bind(secondarySocket, (struct sockaddr*)&stAddr2, sizeof(struct sockaddr));
    if (nBind < 0)
    {
       ui->textEditLog->appendPlainText(QString("Can't bind a name to a socket.\n"));
       exit(1);
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents);

    // Load level
    QString levelName = ui->comboBoxLevel->currentText();
    m_level = new Level();
    glm::vec2 playerPos = m_level->load("Levels/" + levelName.toStdString());
    ui->textEditLog->appendPlainText(QString("Loaded level.\n"));
    m_player1.init(playerPos, glm::vec2(PLAYER_SIZE));
    m_player2.init(playerPos, glm::vec2(PLAYER_SIZE));


    char buf[1];
    buf[0]='n';
    struct sockaddr_in p1_addr, p2_addr;
    unsigned int  slen=sizeof(stAddr);
    slen = sizeof(struct sockaddr_in);
    ui->textEditLog->appendPlainText(QString("Waiting for clients."));


    while (buf[0]!='c')//nie otrzymano prośby polaczenia
    {
        recvfrom(m_mainSocket, buf, 1, MSG_DONTWAIT, (struct sockaddr*)&p1_addr ,&slen);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        sleep(1);
    }
    buf[0]='1';
    ui->textEditLog->appendPlainText(QString("First player connected: ") + inet_ntoa(p1_addr.sin_addr));


    QCoreApplication::processEvents(QEventLoop::AllEvents);


    while (buf[0]!='c')//nie otrzymano prośby polaczenia
    {
        recvfrom(m_mainSocket, buf, 1, MSG_DONTWAIT, (struct sockaddr*)&p2_addr , &slen);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        sleep(1);
    }
    char buf2[1];
    buf2[0]='2';
    ui->textEditLog->appendPlainText(QString("Second player connected: ") + inet_ntoa(p2_addr.sin_addr));

    // Give players their numbers
    sendto(m_mainSocket, buf2, strlen(buf2), 0, (struct sockaddr*) &p2_addr, slen);
    sendto(m_mainSocket, buf, strlen(buf), 0, (struct sockaddr*) &p1_addr, slen);

    // Sen client level name (it should have the same level files)
    sendto(m_mainSocket, levelName.toStdString().c_str(), levelName.length(), 0, (struct sockaddr*) &p1_addr, slen);
    sendto(secondarySocket, levelName.toStdString().c_str(), levelName.length(), 0, (struct sockaddr*) &p2_addr, slen);
    ui->textEditLog->appendPlainText(QString("Waiting for readiness"));

    QCoreApplication::processEvents(QEventLoop::AllEvents);

    //Clients load levels and initialize players
    //Wait until they are ready

    buf2[0]='0';
    while (buf[0]!='r' || buf2[0]!='r'){
        if (buf[0]!='r'){
            recvfrom(m_mainSocket, buf, 1, MSG_DONTWAIT, (struct sockaddr*)&p1_addr ,&slen);
            if (buf[0]=='r'){
                ui->textEditLog->appendPlainText(QString("First player ready!"));
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            }
        }
        if (buf2[0]!='r'){
            recvfrom(secondarySocket, buf2, 1, MSG_DONTWAIT, (struct sockaddr*)&p2_addr ,&slen);
            if (buf2[0]=='r') {
                ui->textEditLog->appendPlainText(QString("Second player ready!"));
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            }
        }

    }

    ui->textEditLog->appendPlainText(QString("The game has started!  "));
    //Send 'go' signal to clientss
    buf[0]='g';
    sendto(m_mainSocket, buf, strlen(buf), 0, (struct sockaddr*) &p1_addr, slen);
    sendto(secondarySocket, buf, strlen(buf), 0, (struct sockaddr*) &p2_addr, slen);

    int i=1;
    float x[4];
    char p1button;
    char p2button;
    char charbuf;
    while(m_isRunning)
    {
        bool p1jump = false;
        bool p2jump = false;
        // Keeps gui responsive
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        i++;
        m_timer.start();

        // Send players their positions; 'if' is useless now
        if (i%1==0){
            i=0;
            x[0]=m_player1.getPosition().x;
            x[1]=m_player1.getPosition().y;
            x[2]=m_player2.getPosition().x;
            x[3]=m_player2.getPosition().y;
            sendto(m_mainSocket, x, 4*sizeof(float), MSG_DONTWAIT, (struct sockaddr*) &p1_addr, slen);
            x[0]=m_player2.getPosition().x;
            x[1]=m_player2.getPosition().y;
            x[2]=m_player1.getPosition().x;
            x[3]=m_player1.getPosition().y;
            sendto(secondarySocket, x, 4*sizeof(float), MSG_DONTWAIT, (struct sockaddr*) &p2_addr, slen);
        }
        p1button = 'x';
        p2button = 'x';
        // Read input
        while (recvfrom(m_mainSocket, &charbuf, sizeof(char), MSG_DONTWAIT, (struct sockaddr*)&p1_addr ,&slen)>0){
             p1button=charbuf;
             if(p1button == 'w')
                 p1jump=true;
        }
        while (recvfrom(secondarySocket, &charbuf, sizeof(char), MSG_DONTWAIT, (struct sockaddr*)&p2_addr ,&slen)>0){
            p2button=charbuf;
            if(p2button == 'w')
                p2jump=true;
        }
        // Calculate physics
        while(m_timer.canGetTimeChunk())
        {
            update(m_timer.getTimeChunk(),p1button,p2button, p1jump, p2jump);
            // Respawn player if he falls
            if(m_level->isBelowLevel(m_player1.getPosition()))
                    m_player1.returnToLastGround();
            if(m_level->isBelowLevel(m_player2.getPosition()))
                    m_player2.returnToLastGround();
            // Check if the game should finish
            if(m_player1.intersects(m_level->getFinishPoint()) || p1button=='q' ||
                    p2button=='q' || m_player2.intersects(m_level->getFinishPoint())){
                x[0]=666;
                x[1]=666;
                x[2]=666;
                x[3]=666;
                sendto(m_mainSocket, x, 4*sizeof(float), MSG_DONTWAIT, (struct sockaddr*) &p1_addr, slen);
                x[0]=666;
                x[1]=666;
                x[2]=666;
                x[3]=666;
                sendto(secondarySocket, x, 4*sizeof(float), MSG_DONTWAIT, (struct sockaddr*) &p2_addr, slen);

                ui->textEditLog->appendPlainText(QString("Someone won (or left :()!"));
                m_isRunning = false;
            }
        }


        m_timer.stop();
        // Limit to 60fps
        QThread::msleep(m_timer.getSleepTime());
    }
    if(m_level)
        delete m_level;
    ::close(m_mainSocket);
}

void MainWindow::update(float deltaTime, char p1button, char p2button, bool p1jump, bool p2jump)
{
    m_player1.update(deltaTime, *m_level, p1button, p1jump);
    m_player2.update(deltaTime, *m_level, p2button, p2jump);
}

void MainWindow::on_buttonStart_clicked()
{
    m_isRunning = true;
    ui->textEditLog->appendPlainText(QString("Server started.\n"));
    startLoop();
    ui->textEditLog->appendPlainText(QString("Server stopped.\n"));
}

void MainWindow::on_buttonStop_clicked()
{
    m_isRunning = false;
    ui->textEditLog->appendPlainText(QString("Server should stop.\n"));
}

void MainWindow::on_buttonRestart_clicked()
{
    on_buttonStop_clicked();
    on_buttonStart_clicked();
}
