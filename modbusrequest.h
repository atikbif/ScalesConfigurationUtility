#ifndef MODBUSREQUEST_H
#define MODBUSREQUEST_H

#include <QByteArray>
#include <QVector>

class ModbusRequest
{
private:
    ModbusRequest();
    static const quint16 wCRCTable[];
    static quint16 getCRC(QByteArray &data);
public:
    static QByteArray getWriteRequest(int netAddr, int memAddr, int regCnt, const QVector<quint16> &regs);
    static QByteArray getReadRequest(int netAddr, int memAddr, int regCnt);
    static QVector<quint16> getDataFromReadAnswer(QByteArray &data);
};

#endif // MODBUSREQUEST_H
