//#include "startwindow.h"
//#include "ui_startwindow.h"
//#include <stdio.h>
//#include <sys/types.h>
//#include <ifaddrs.h>
//#include "startwindow.h"
//#include "ui_startwindow.h"

////startWindow::startWindow(QWidget *parent) :
////    QDialog(parent),
////    ui(new Ui::startWindow)
////{
////    ui->setupUi(this);
////}

////startWindow::~startWindow()
////{
////    delete ui;
////}

//startwindow::startwindow(QWidget *parent) :
//    QDialog(parent)
//{

//    ui->setupUi(this);
//    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
//    this->show_startWindow();
//}

//startwindow::~startwindow()
//{
//    delete ui;
//}

//void startwindow::on_startButton_clicked()
//{
//    //QByteArray ba = ui->textEdit->toPlainText().toUtf8();
//    this->selectedInterface = ui->textEdit->toPlainText();//ba.data();
//    this->close();
//    emit startwindowClosed();
//}

//void startwindow::show_startWindow(){
//    struct ifaddrs *addrs;

//    getifaddrs(&addrs);

//    while (addrs)
//    {
//        if (addrs->ifa_addr && addrs->ifa_addr->sa_family == AF_PACKET)
//            ui->interfacelist->addItem(addrs->ifa_name);

//        addrs = addrs->ifa_next;
//    }

//    freeifaddrs(addrs);
//}

//void startwindow::on_interfacelist_itemClicked(QListWidgetItem *item)
//{
//    ui->textEdit->setText(ui->interfacelist->currentItem()->text());
//}
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include "startwindow.h"
#include "ui_startwindow.h"

startwindow::startwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startwindow)
{
    ui->setupUi(this);
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    this->show_startWindow();
}

startwindow::~startwindow()
{
    delete ui;
}

void startwindow::on_startButton_clicked()
{
    //QByteArray ba = ui->textEdit->toPlainText().toUtf8();
    this->selectedInterface = ui->textEdit->toPlainText();//ba.data();
    this->close();
    emit startwindowClosed();
}

void startwindow::show_startWindow(){
    struct ifaddrs *addrs;

    getifaddrs(&addrs);

    while (addrs)
    {
        if (addrs->ifa_addr && addrs->ifa_addr->sa_family == AF_PACKET)
            ui->interfacelist->addItem(addrs->ifa_name);

        addrs = addrs->ifa_next;
    }

    freeifaddrs(addrs);
}

void startwindow::on_interfacelist_itemClicked(QListWidgetItem *item)
{
    ui->textEdit->setText(ui->interfacelist->currentItem()->text());
}
