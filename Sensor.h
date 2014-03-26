#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor
{
public:
    Sensor(const QString &sensorName, const QString &portName);

    QString sensorName() const {
        return mSensorName;
    }

    QString portName() const {
        return mPortName;
    }

    int value() const;

private:
    QString mSensorName;
    QString mPortName;
};

#endif // SENSOR_H
