#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pkt.h"
#include <QMainWindow>
#include "interfacechoose.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    my_packet * Packet;

    std::map<unsigned int, info> AP_INFO;
    std::map<unsigned int, info> Client_INFO;
    int total_packet = 0;
    int Stop = 0;

private slots:
    void on_start_btn_clicked();
    void run();
    void on_stop_btn_clicked();

private:
    Ui::MainWindow *ui;
    interfacechoose *choose;


};

#endif // MAINWINDOW_H
