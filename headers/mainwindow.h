#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../headers/tcpclient.h"
#include "../headers/about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_pb_connect_clicked();
    void on_pb_request_clicked();

    void DisplayTime(QDateTime time);
    void DisplayFreeSpace(uint32_t freeSpace);
    void SetDataReply(QString);
    void DisplayStat(StatServer);
    void DisplayConnectStatus(uint16_t status);
    void DisplayError(uint16_t error);
    void DisplaySuccess(uint16_t typeMess);

    void on_cb_request_currentIndexChanged(int index);
    void on_action_exit_triggered();
    void on_action_about_triggered();
    void on_action_connect_triggered();

private:

    Ui::MainWindow * ui;
    TCPclient * client;
    About * about;
    QMovie mo_cat;
};
#endif // MAINWINDOW_H
