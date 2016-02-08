#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QSettings>

#include "dialogselectport.h"
#include "ui_dialogselectport.h"

DialogSelectPort::DialogSelectPort(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogSelectPort) {
  ui->setupUi(this);

  QSettings settings("settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("Interface");
  currentPort = settings.value("PortName").toString();
  settings.endGroup();
}

DialogSelectPort::~DialogSelectPort() {
  delete ui;
}

QString DialogSelectPort::getPortName() {
  return(currentPort);
}

void DialogSelectPort::refreshPorts() {
  QSettings settings("settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("Interface");
  currentPort = settings.value("PortName").toString();
  settings.endGroup();

  ui->labelCurrentPort->setText(currentPort);

  QList<QSerialPortInfo> list;
  list = QSerialPortInfo::availablePorts();

  ui->tableWidgetPorts->setRowCount(list.count());

  ui->tableWidgetPorts->clearContents();

  ui->tableWidgetPorts->horizontalHeader()->resizeSection(0, 80);
  ui->tableWidgetPorts->horizontalHeader()->resizeSection(1, 150);
  ui->tableWidgetPorts->horizontalHeader()->resizeSection(2, 200);
  ui->tableWidgetPorts->horizontalHeader()->resizeSection(3, 50);

  for (unsigned char i=0;i<list.count();i++) {
    QTableWidgetItem *newItem = new QTableWidgetItem(list.at(i).portName());
    ui->tableWidgetPorts->setItem(i, 0, newItem);

    newItem = new QTableWidgetItem(list.at(i).manufacturer());
    ui->tableWidgetPorts->setItem(i, 1, newItem);

    newItem = new QTableWidgetItem(list.at(i).description());
    ui->tableWidgetPorts->setItem(i, 2, newItem);

    if (list.at(i).isBusy())
      newItem = new QTableWidgetItem((tr("Yes")));
    else
      newItem = new QTableWidgetItem((tr("No")));

    ui->tableWidgetPorts->setItem(i, 3, newItem);


    if (list.at(i).portName() == currentPort)
      ui->tableWidgetPorts->setCurrentCell(i,0);
  }
}

void DialogSelectPort::on_pushButtonRefresh_clicked() {
  refreshPorts();
}

void DialogSelectPort::on_pushButtonCancel_clicked() {
  hide();
}

void DialogSelectPort::on_pushButtonOK_clicked() {
  QList<QTableWidgetItem*> list;
  list = ui->tableWidgetPorts->selectedItems();

  QSettings settings("settings.ini",QSettings::IniFormat,0);

  if (list.count() == 0) {  //Empty
    ui->labelCurrentPort->setText("Undefined");

    settings.beginGroup("Interface");
    settings.setValue("PortName","Undefined");
    settings.endGroup();
  }
  else { //We found items, lets see which port
    currentPort = list.at(0)->text();
    ui->labelCurrentPort->setText(currentPort);

    settings.beginGroup("Interface");
    settings.setValue("PortName",currentPort);
    settings.endGroup();
  }

  hide();
}
