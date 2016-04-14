#include "scanmanager.h"

ScanManager::ScanManager(Scales &scales, QObject *parent) : QObject(parent), sc(scales)
{
    scan = new Scanner(sc);
    scan->moveToThread(&thread);
    connect(scan,SIGNAL(confUpdate()),this,SIGNAL(confUpdate()));
    connect(&thread,SIGNAL(finished()),scan,SLOT(deleteLater()));
    connect(this,SIGNAL(startScan()),scan,SLOT(start()));
    connect(scan,SIGNAL(correctAnswer()),this,SIGNAL(correctAnswer()));
    connect(scan,SIGNAL(errorMessage(QString)),this,SIGNAL(errorMessage(QString)));
    thread.start();
    emit startScan();
}

ScanManager::~ScanManager()
{
    scan->finish();
    thread.quit();
    thread.wait();
}

void ScanManager::setScanFlag(bool value)
{
    scan->setScanFlag(value);
}

void ScanManager::setPortName(const QString pName)
{
    scan->setPortName(pName);
}

void ScanManager::readConf()
{
    scan->readConf();
}

void ScanManager::writeConf()
{
    scan->writeConf();
}
