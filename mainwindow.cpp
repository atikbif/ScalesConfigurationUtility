#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), calibr1(0), calibr2(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanMan = new ScanManager(sc);
    on_pushButtonPortListUpdate_clicked();
    connect(&sc,SIGNAL(adcUpdate(uint)),this,SLOT(adcUpdate(uint)));
    connect(&sc,SIGNAL(weightUpdate(uint)),this,SLOT(weightUpdate(uint)));
    connect(scanMan,SIGNAL(confUpdate()),this,SLOT(confUpdate()));
    connect(scanMan,SIGNAL(correctAnswer()),this,SLOT(correctAnswer()));
    connect(scanMan,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scanMan;
}

void MainWindow::on_pushButtonPortListUpdate_clicked()
{
    ui->comboBoxCOM->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxCOM->addItem(info.portName());
    }
}

void MainWindow::on_pushButtonScan_clicked()
{
    if(ui->pushButtonScan->text().contains("стоп")) {
        ui->labelMessage->clear();
        ui->pushButtonScan->setText("старт");
        scanMan->setScanFlag(false);
        ui->pushButtonCalibr1->setEnabled(false);
        ui->pushButtonCalibr2->setEnabled(false);
        ui->pushButtonReadSettings->setEnabled(false);
        ui->pushButtonWriteSettings->setEnabled(false);
    }else {
        ui->pushButtonScan->setText("стоп");
        scanMan->setPortName(ui->comboBoxCOM->currentText());
        scanMan->setScanFlag(true);
        ui->pushButtonCalibr1->setEnabled(true);
        ui->pushButtonCalibr2->setEnabled(true);
        ui->pushButtonReadSettings->setEnabled(true);
        ui->pushButtonWriteSettings->setEnabled(true);
    }
    ui->pushButtonScan->setStyleSheet(ui->pushButtonPortListUpdate->styleSheet());
}

void MainWindow::on_pushButtonCalibr1_clicked()
{
    calibr1 = sc.getADC();
    QMessageBox::information(this, tr("калибровка"), tr("Данные приняты.\r\n Установите следующий калибровочный вес.") );
}

void MainWindow::on_pushButtonCalibr2_clicked()
{
    calibr2 = sc.getADC();
    if(calibr2>calibr1) {
        double k = ui->spinBoxCalibr2->value() - ui->spinBoxCalibr1->value();
        k = k/(calibr2-calibr1);
        if(k>0) {
            unsigned long plWeight = (unsigned long)ui->spinBoxCalibr2->value()*calibr1;
            plWeight -= (unsigned long)ui->spinBoxCalibr1->value()*calibr2;
            plWeight = plWeight/(calibr2-calibr1);
            sc.getSettings().setCoeff(k);
            sc.getSettings().setPlatformWeight(plWeight);
            confUpdate();
            scanMan->writeConf();
            QMessageBox::information(this, tr("калибровка"), tr("Калибровка успешно завершена.") );
        }else QMessageBox::information(this, tr("калибровка"), tr("Некорректные входные данные.") );
    }else QMessageBox::information(this, tr("калибровка"), tr("Некорректные входные данные.") );
}

void MainWindow::on_pushButtonReadSettings_clicked()
{
    scanMan->readConf();
}

void MainWindow::on_pushButtonWriteSettings_clicked()
{
    sc.getSettings().setCoeff(ui->doubleSpinBoxCoeff->value());
    sc.getSettings().setNetAddress(ui->spinBoxNetAddr->value());
    sc.getSettings().setPlatformWeight(ui->spinBoxPlatformWeight->value());
    scanMan->writeConf();
}

void MainWindow::confUpdate()
{
    ui->spinBoxNetAddr->setValue(sc.getSettings().getNetAddress());
    ui->doubleSpinBoxCoeff->setValue(sc.getSettings().getCoeff());
    ui->spinBoxPlatformWeight->setValue(sc.getSettings().getPlatformWeight());
}

void MainWindow::weightUpdate(unsigned int value)
{
    ui->lcdNumbeWeight->display((int)value);
}

void MainWindow::adcUpdate(unsigned int value)
{
    ui->lcdNumberADC->display((int)value);
}

void MainWindow::correctAnswer()
{
    if(ui->pushButtonScan->text().contains("стоп")) {
        ui->pushButtonScan->setStyleSheet("background-color: rgb(0,200,0);");
        ui->labelMessage->setText("опрос устройства");
    }
}

void MainWindow::errorMessage(QString message)
{
    if(ui->pushButtonScan->text().contains("стоп")) {
        ui->pushButtonScan->setStyleSheet("background-color: rgb(200,0,0);");
        ui->labelMessage->setText(message);
    }
}
