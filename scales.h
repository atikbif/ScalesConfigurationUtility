#ifndef SCALES_H
#define SCALES_H

#include <QObject>
#include "scalessettings.h"

class Scales : public QObject
{
    Q_OBJECT
    ScalesSettings conf;
    unsigned int weight;
    unsigned int adc;
public:
    explicit Scales(QObject *parent = 0);
    unsigned int getWeight() const {return weight;}
    unsigned int getADC() const {return adc;}
    void setWeight(unsigned int value) {weight = value; emit weightUpdate(weight);}
    void setADC(unsigned int value) {adc = value; emit adcUpdate(adc);}
    ScalesSettings &getSettings() {return conf;}
signals:
    weightUpdate(unsigned int);
    adcUpdate(unsigned int);
public slots:
};

#endif // SCALES_H
