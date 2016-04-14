#ifndef SCANMANAGER_H
#define SCANMANAGER_H

#include <QObject>
#include <QThread>
#include "scanner.h"
#include "scales.h"

class ScanManager : public QObject
{
    Q_OBJECT
    QThread thread;
    Scanner *scan;
    Scales &sc;
public:
    explicit ScanManager(Scales &scales, QObject *parent = 0);
    ~ScanManager();
    void setScanFlag(bool value);
    void setPortName(const QString pName);
    void readConf();
    void writeConf();

signals:
    startScan();
    confUpdate();
    correctAnswer();
    errorMessage(QString message);
public slots:
};

#endif // SCANMANAGER_H
