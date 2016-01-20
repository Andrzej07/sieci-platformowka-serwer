#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void findLevels();
    int createSocket();
    void configSocket(int sock);

    bool isRunning = false;
    int mainSocket;
};

#endif // MAINWINDOW_H
