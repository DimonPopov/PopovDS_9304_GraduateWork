#include <QTimer>
#include <random>
#include "sensor.h"


SensorSpace::Sensor::Sensor(const quint32 position, QObject* parent)
    : QObject(parent),
      m_positionInArray(position),
      m_updateDataTimer(new QTimer(this))
{
    m_updateDataTimer->setInterval(200);

    connect(m_updateDataTimer, &QTimer::timeout,
            this, &Sensor::handleTimerTimeout);
}

void SensorSpace::Sensor::setTimerStatus(bool status)
{
    if (m_updateDataTimer)
        status ? m_updateDataTimer->start() : m_updateDataTimer->stop();
}

void SensorSpace::Sensor::handleTimerTimeout()
{

    float a_random_double = float(rand() % 10);
    emit sigSensorUpdateData(m_positionInArray, QVector3D(a_random_double,
                                                          a_random_double,
                                                          a_random_double));
}

QVector3D &SensorSpace::getSensorPosition(const Sensor* sensor)
{

}
