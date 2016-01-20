#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Timer.h>
#include <Level.h>
#include <Player.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonStart_clicked();

    void on_buttonStop_clicked();

private:
    Ui::MainWindow *ui;

    void update(float deltaTime);
    void findLevels();
    int createSocket();
    void configSocket(int sock);
    void startLoop();

    bool m_isRunning = false;
    int m_mainSocket;
    Timer m_timer;
    Level m_level;
    Player m_player1;
    Player m_player2;
};

#endif // MAINWINDOW_H
