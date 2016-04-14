#include "scanner.h"
#include <QMutexLocker>
#include "modbusrequest.h"
#include <QVector>
#include <QThread>

Scanner::Scanner(Scales &scales, QObject *parent) : QObject(parent),
    scanFlag(false), finishFlag(false), sc(scales), portName("COM1"),
    readConfFlag(false), writeConfFlag(false)
{

}

void Scanner::setPortName(const QString pName)
{
    QMutexLocker locker(&mutex);
    portName = pName;
}

void Scanner::readConf()
{
    QMutexLocker locker(&mutex);
    readConfFlag = true;
}

void Scanner::writeConf()
{
    QMutexLocker locker(&mutex);
    writeConfFlag = true;
}

void Scanner::start()
{
    forever {
        mutex.lock();
        if(finishFlag) {
            mutex.unlock();
            break;
        }
        if(scanFlag) {
            mutex.unlock();
            // опрос устройства
            QSerialPort port;
            if(openPort(portName,port)) {
                mutex.lock();
                if(writeConfFlag) {
                    writeConfFlag = false;
                    QVector<quint16> wrData;
                    wrData.resize(5);
                    wrData[0] = sc.getSettings().getNetAddress();
                    wrData[3] = sc.getSettings().getPlatformWeight();
                    wrData[4] = (unsigned int)(sc.getSettings().getCoeff()*10000 + 0.5);
                    QByteArray data;
                    data = ModbusRequest::getWriteRequest(0,0,5,wrData);
                    writeAndRead(data,port);
                    thread()->msleep(10);
                }
                mutex.unlock();
                QByteArray data;
                data = ModbusRequest::getReadRequest(0,0,10);
                writeAndRead(data, port);
                if(!data.isEmpty()) {
                    QVector<quint16> res = ModbusRequest::getDataFromReadAnswer(data);
                    if(res.count()) {
                        emit correctAnswer();
                        sc.setADC(res.at(1));
                        sc.setWeight(res.at(2));
                        mutex.lock();
                        if(readConfFlag) {
                            readConfFlag = false;
                            sc.getSettings().setNetAddress(res.at(0));
                            sc.getSettings().setCoeff((double)res.at(4)/10000);
                            sc.getSettings().setPlatformWeight(res.at(3));
                            emit confUpdate();
                        }
                        mutex.unlock();
                    }else emit errorMessage("Некорректный ответ");
                }else emit errorMessage("Контроллер не отвечает");
                port.close();
            }else emit errorMessage("Ошибка открытия порта");
        }else mutex.unlock();
        thread()->msleep(10);
    }
}

void Scanner::setScanFlag(bool value)
{
    QMutexLocker locker(&mutex);
    scanFlag = value;
}

void Scanner::finish()
{
    QMutexLocker locker(&mutex);
    finishFlag = true;
}

bool Scanner::openPort(const QString &pName, QSerialPort &port)
{
    port.setPortName(pName);
    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);
    if(port.open(QSerialPort::ReadWrite)) return true;
    return false;
}

void Scanner::writeAndRead(QByteArray &data, QSerialPort &port)
{
    port.write(data);
    port.waitForBytesWritten(50);
    data.clear();
    if(port.waitForReadyRead(50)){
        int cnt=0;
        data+=port.readAll();
        while(port.waitForReadyRead(10)) {
            data+=port.readAll();
            cnt++;if(cnt>=500) break;
        }
    }
}
