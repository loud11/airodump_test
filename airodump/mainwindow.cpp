#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <map>
#include <iostream>
#include <QtCore>
#include <QMessageBox>
#include <string>
#include "pkt.h"
using namespace std;

#define BSSID_col 0
#define CHANNEL_col 1
#define POWER_col 2
#define DATA_col 3
#define BEACON_col 4
#define ESSID_col 5

///////////////////////////////////////////

uint32_t SuperFastHash (uint8_t *mac) {
    unsigned int hash=0;
    unsigned int tmp=0;
    /* Main loop */
    for (int i=0;i < 6; i++) {
        hash  += (mac[i]);
        tmp    = ((mac[i]+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        hash  += hash >> 11;
    }

    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

///////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    choose = new interfacechoose(this);
    ui->setupUi(this);
    connect(choose,SIGNAL(startwindowClosed()),this,SLOT(run()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run(){
    const u_char *packet;
    pcap_t *handle;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header;
    info AP_INFO_STORE;

    dev = (char *)choose->get_selectedInterface();

    handle = pcap_open_live(dev, BUFSIZ, 1, 100, errbuf);


    if (pcap_datalink(handle) != DLT_IEEE802_11_RADIO){ // DLT_IEEE802_11_RADIO = 127

        QMessageBox::warning(this, tr("Solve this!"),
                             tr("This don't match with IEEE802_11_RADIO.\n"
                                "Please select one supports IEEE802_11_RADIO."),QMessageBox::Yes);

        return;
    }

    this->total_packet = 0;
    ui->treeWidget->clear();


    while(1){

        QMutex mutex;
        mutex.lock();
        if(this->Stop == 1) break;

        if(pcap_next_ex(handle, &header, &packet) == 1){ // was next_ex successful
            Packet = new my_packet(packet);

            AP_INFO_STORE.channel = ((Packet->RT->channel_frequency - 2412)/5 + 1);
            AP_INFO_STORE.channel_power = (256 - Packet->RT->ssi_signal);

            switch(Packet->flag){

            case BEACON_FRAME:
            {

                memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr2 , 6);

                char buffer[18];
                snprintf(buffer,sizeof(buffer),"%02x:%02x:%02x:%02x:%02x:%02x",AP_INFO_STORE.bssid[0],AP_INFO_STORE.bssid[1],AP_INFO_STORE.bssid[2],AP_INFO_STORE.bssid[3],AP_INFO_STORE.bssid[4],AP_INFO_STORE.bssid[5]);
                std::string bufferasstring = buffer;

                QTreeWidgetItem *group;
                QList<QTreeWidgetItem*> items = (ui->treeWidget)->findItems(QString::fromStdString(bufferasstring), Qt::MatchExactly, BSSID_col); //bssid searching

                //#define BSSID_col 0
                //#define CHANNEL_col 1
                //#define POWER_col 2
                //#define DATA_col 3
                //#define BEACON_col 4
                //#define ESSID_col 5

                if (items.count() == 0) {   // not found, create new entry

                    group = new QTreeWidgetItem((ui->treeWidget));
                    group->setText(BSSID_col, QString::fromStdString(bufferasstring));
                    group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                    group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                    group->setText(DATA_col , QString::number(0));
                    group->setText(BEACON_col , QString::number(1));
                    strncpy(AP_INFO_STORE.essid, (char *)(packet + Packet->RT->it_len + sizeof(ie_de_head) + sizeof(ie_frame_fixedPara) + 2),Packet->ie_frame->SSID_length);
                    AP_INFO_STORE.essid[Packet->ie_frame->SSID_length] = '\0';
                    bufferasstring = AP_INFO_STORE.essid;
                    group->setText(ESSID_col, QString::fromStdString(bufferasstring));

                }
                else
                {

                    group = items.at(0);
                    group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                    group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                    group->setText(BEACON_col , QString::number(group->text(BEACON_col).toInt()+1));

                }

                //                printf("this is %s \n" , AP_INFO_STORE.essid);
                //                for(int i = 0 ; i < 6 ; i++) printf("[%02x]",AP_INFO_STORE.bssid[i]);
                //                printf("\n");
                break;
            }

            case PROBE_RESPONSE:
            {
                memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr2 , 6); // bssid
                char buffer[18];
                snprintf(buffer,sizeof(buffer),"%02x:%02x:%02x:%02x:%02x:%02x",AP_INFO_STORE.bssid[0],AP_INFO_STORE.bssid[1],AP_INFO_STORE.bssid[2],AP_INFO_STORE.bssid[3],AP_INFO_STORE.bssid[4],AP_INFO_STORE.bssid[5]);
                std::string bufferasstring = buffer;

                memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr1 , 6); // station
                snprintf(buffer,sizeof(buffer),"%02x:%02x:%02x:%02x:%02x:%02x",AP_INFO_STORE.bssid[0],AP_INFO_STORE.bssid[1],AP_INFO_STORE.bssid[2],AP_INFO_STORE.bssid[3],AP_INFO_STORE.bssid[4],AP_INFO_STORE.bssid[5]);
                std::string bufferasstring2 = buffer;



                QTreeWidgetItem *group;
                QList<QTreeWidgetItem*> items = (ui->treeWidget)->findItems(QString::fromStdString(bufferasstring2), Qt::MatchExactly|Qt::MatchRecursive, BSSID_col); //bssid searching

                if (items.isEmpty() == true) {   // not found, create new entry
                    QList<QTreeWidgetItem*> items1 = (ui->treeWidget)->findItems(QString::fromStdString(bufferasstring), Qt::MatchExactly, BSSID_col); //bssid searching
                    if(items1.isEmpty() == true){
                        group = new QTreeWidgetItem((ui->treeWidget));
                        group->setText(BSSID_col, QString::fromStdString(bufferasstring));
                        group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                        group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                        group->setText(DATA_col , QString::number(0));
                        group->setText(BEACON_col , QString::number(1));
                        strncpy(AP_INFO_STORE.essid, (char *)(packet + Packet->RT->it_len + sizeof(ie_de_head) + sizeof(ie_frame_fixedPara) + 2),Packet->ie_frame->SSID_length);
                        AP_INFO_STORE.essid[Packet->ie_frame->SSID_length] = '\0';
                        std::string bufferasstring3 = AP_INFO_STORE.essid;
                        group->setText(ESSID_col, QString::fromStdString(bufferasstring3));

                        QTreeWidgetItem *child1 = new QTreeWidgetItem();;
                        child1->setText(BSSID_col,QString::fromStdString(bufferasstring2));
                        group->addChild(child1);

                    }else{
                        group = items1.at(0);

                        QTreeWidgetItem *child1 = new QTreeWidgetItem();;
                        child1->setText(BSSID_col,QString::fromStdString(bufferasstring2));
                        child1->setText(CHANNEL_col,QString::number(AP_INFO_STORE.channel));
                        child1->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                        child1->setText(DATA_col , QString::number(0));

                        group->addChild(child1);
                    }


                }else{
                    group = items.at(0);
                    group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                    group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                    group->setText(DATA_col , QString::number(group->text(DATA_col).toInt()+1));
                }


                break;
            }

            case DATA_FRAME:
            {
                switch((Packet->ie_header->frame_control_field)& ((1 << 2) - 1)){
                case 0x00:
                    memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr3 , 6);
                    break;
                case 0x01:
                    memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr1 , 6);
                    break;
                case 0x02:
                    memcpy(AP_INFO_STORE.bssid, Packet->ie_header->addr2 , 6);
                    break;
                }
                char buffer[18];
                snprintf(buffer,sizeof(buffer),"%02x:%02x:%02x:%02x:%02x:%02x",AP_INFO_STORE.bssid[0],AP_INFO_STORE.bssid[1],AP_INFO_STORE.bssid[2],AP_INFO_STORE.bssid[3],AP_INFO_STORE.bssid[4],AP_INFO_STORE.bssid[5]);
                std::string bufferasstring = buffer;

                QTreeWidgetItem *group;
                QList<QTreeWidgetItem*> items = (ui->treeWidget)->findItems(QString::fromStdString(bufferasstring), Qt::MatchExactly, BSSID_col); //bssid searching

                if (items.count() == 0) {   // not found, create new entry

                    group = new QTreeWidgetItem((ui->treeWidget));
                    group->setText(BSSID_col, QString::fromStdString(bufferasstring));
                    group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                    group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                    group->setText(DATA_col , QString::number(1));
                    group->setText(BEACON_col , QString::number(0));
                    strncpy(AP_INFO_STORE.essid, (char *)(packet + Packet->RT->it_len + sizeof(ie_de_head) + sizeof(ie_frame_fixedPara) + 2),Packet->ie_frame->SSID_length);
                    AP_INFO_STORE.essid[Packet->ie_frame->SSID_length] = '\0';
                    bufferasstring = AP_INFO_STORE.essid;
                    group->setText(ESSID_col, QString::fromStdString(bufferasstring));

                }
                else
                {

                    group = items.at(0);
                    group->setText(CHANNEL_col , QString::number(AP_INFO_STORE.channel));
                    group->setText(POWER_col , QString::number(AP_INFO_STORE.channel_power));
                    group->setText(DATA_col , QString::number(group->text(DATA_col).toInt()+1));

                }


                break;

            }

            }

            delete Packet;

            total_packet++;
            ui->test_text->setText(QString::number(total_packet));

            qApp->processEvents();

            mutex.unlock();

        }else{ // next_ex was not successful
            continue;
        }

    }

    pcap_close(handle);
    return;

}

void MainWindow::on_start_btn_clicked()
{
    this->Stop = 0;
    choose->show();

    //    this->run();
}

void MainWindow::on_stop_btn_clicked()
{
    this->Stop = 1;
}
