#include "interfacechoose.h"
#include "ui_interfacechoose.h"

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>


interfacechoose::interfacechoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interfacechoose)
{
    ui->setupUi(this);
    //    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    //    this->show_startWindow();
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    this->show_startWindow();

}

interfacechoose::~interfacechoose()
{
    delete ui;
}

void interfacechoose::show_startWindow(){

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

void interfacechoose::on_interfacelist_clicked(const QModelIndex &index)
{
    ui->textEdit->setText(ui->interfacelist->currentItem()->text());
}

void interfacechoose::on_startButton_clicked()
{
    QByteArray ba = ui->textEdit->toPlainText().toUtf8();
    this->selectedInterface = ba.data();
    this->close();
    emit startwindowClosed();
}
