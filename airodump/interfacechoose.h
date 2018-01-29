#ifndef INTERFACECHOOSE_H
#define INTERFACECHOOSE_H

#include <QDialog>

namespace Ui {
class interfacechoose;
}

class interfacechoose : public QDialog
{
    Q_OBJECT

public:
    explicit interfacechoose(QWidget *parent = 0);
    ~interfacechoose();
    void show_startWindow();
    const char * get_selectedInterface(){ return this->selectedInterface;}

private slots:
    void on_interfacelist_clicked(const QModelIndex &index);

    void on_startButton_clicked();
signals:
    void startwindowClosed();
private:
    Ui::interfacechoose *ui;
    const char * selectedInterface;
};

#endif // INTERFACECHOOSE_H
