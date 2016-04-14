#ifndef SCALESSETTINGS_H
#define SCALESSETTINGS_H

#include <QObject>

class ScalesSettings: public QObject
{
    Q_OBJECT

    unsigned char netAddress;
    unsigned short platformWeight;
    double coeff;
public:
    explicit ScalesSettings(QObject *parent = 0);
    unsigned char getNetAddress() const {return netAddress;}
    unsigned short getPlatformWeight() const {return platformWeight;}
    double getCoeff() const {return coeff;}
    void setNetAddress(unsigned char value) {netAddress = value;}
    void setPlatformWeight(unsigned short value) {platformWeight = value;}
    void setCoeff(double value) {coeff = value;}
};

#endif // SCALESSETTINGS_H
