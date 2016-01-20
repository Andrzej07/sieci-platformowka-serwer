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
#include <arpa/inet.h>
#include <fcntl.h>


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
    const int BUFSIZE = 10;

    char buffer[BUFSIZE];

    int nBind;
    sockaddr_in stAddr;

    mainSocket = createSocket();
    configSocket(mainSocket);

    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    /* bind a name to a socket */
    nBind = bind(mainSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
       ui->textEditLog->appendPlainText(QString("Can't bind a name to a socket.\n"));
       exit(1);
    }


    socklen_t nTmp;
    sockaddr_in stClientAddr;
    nTmp = sizeof(stClientAddr);

    /*
     * float number = 123.2342;
     * send(mainSocket, &number, sizeof(float),0);
     */
    while(isRunning)
    {
        rcvd = recv(mainSocket, buffer, 1024, 0);
        send(mainSocket, buffer, rcvd, 0);
        QCoreApplication::processEvents();
    }
    // Uwaga, nie zamykamy
    // Chyba lepiej uzyc threadingu od Qt
    ::close(mainSocket);
}

void MainWindow::on_buttonStart_clicked()
{
    isRunning = true;
    ui->textEditLog->appendPlainText(QString("Server started.\n"));
    startLoop();
    ui->textEditLog->appendPlainText(QString("Server stopped.\n"));
}

void MainWindow::on_buttonStop_clicked()
{
    isRunning = false;
    ui->textEditLog->appendPlainText(QString("Server should stop.\n"));
}
