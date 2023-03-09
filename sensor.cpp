#include <QTimer>
#include <random>
#include <qmath.h>
#include <QDebug>

#include "sensor.h"


SensorSpace::Sensor::Sensor(const quint32 position, SensorModel* model, QObject* parent)
    : QObject(parent),
      m_positionInArray(position),
      m_model(model),
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
    emit sigSensorUpdateData(m_positionInArray, m_model->getNewSensorPosition(m_positionInArray, true));
}

SensorSpace::SensorModel::SensorModel(const quint32 &sensorCount, const quint32& enabledSensor)
    : m_sensorCount(sensorCount),
      m_enabledSensor(enabledSensor)
{
    m_step = 10.0 / static_cast<double>(m_enabledSensor);
}

QVector3D SensorSpace::SensorModel::getNewSensorPosition(const quint32 &positionInArray, const bool& timerEnable)
{
    static double f[] = {0, 0.1, 0.03, 0.12,
                         0.14, 0, 0.27, 0.014,
                         0, 0.38, 0.34, 0.09 };
    static int l = 0;

    if (timerEnable)
    {
        ++l;
        if (l == 11)
            l = 0;
    }

    const double newPosition = 3 + sin(positionInArray);

    return QVector3D(m_step * positionInArray + f[l], newPosition + f[l], 2);
}

void SensorSpace::SensorModel::setEnabledSensor(const quint32 &newValue)
{
    m_enabledSensor = newValue;
    m_step = 10.0 / static_cast<double>(m_enabledSensor);
}
