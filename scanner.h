#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QMutex>
#include "scales.h"
#include <QString>
#include <QSerialPort>

class Scanner : public QObject
{
    Q_OBJECT

    mutable QMutex mutex;
    bool scanFlag;
    bool finishFlag;
    Scales &sc;
    QString portName;
    bool readConfFlag;
    bool writeConfFlag;

    bool openPort(const QString &pName, QSerialPort &port);
    void writeAndRead(QByteArray &data, QSerialPort &port);
public:
    explicit Scanner(Scales &scales, QObject *parent = 0);
    void setPortName(const QString pName);
    void readConf();
    void writeConf();
signals:
    errorMessage(QString message);
    confUpdate();
    correctAnswer();
public slots:
    void start();
    void setScanFlag(bool value);
    void finish();
};

#endif // SCANNER_H
