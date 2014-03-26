#include "Sensor.h"

#include <QTime>

Sensor::Sensor(const QString &sensorName, const QString &portName) :
    mSensorName(sensorName),
    mPortName(portName)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

int Sensor::value () const
{
    return qrand() % 10 + 10;
}
