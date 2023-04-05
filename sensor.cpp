#include <QTimer>
#include <random>
#include <qmath.h>
#include <QDebug>

#include "sensor.h"


SensorSpace::Sensor::Sensor(const quint32 position, QSharedPointer<SensorModel> model, QObject* parent)
    : QObject(parent),
      m_positionInArray(position),
      m_model(model),
      m_updateDataTimer(new QTimer(this))
{
    m_updateDataTimer->setInterval(1'000);

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
    emit sigSensorUpdateData(m_positionInArray, m_model.toStrongRef()->getNewSensorPosition(m_positionInArray, true));
}

SensorSpace::SensorModel::SensorModel(const quint32& enabledSensor, const double& lenght)
    : m_enabledSensor(enabledSensor),
      m_len(lenght)
{
    calculateStep();
}

QVector3D SensorSpace::SensorModel::getNewSensorPosition(const quint32 &positionInArray, const bool& timerEnable)
{
    Q_UNUSED(timerEnable);

    if (!m_enabledSensor || !m_len)
        throw std::logic_error("Incorrect sensor model parameters");

    const double x = m_step * positionInArray;
    const double y = 3 + sin(2 * x + 0.5f);
    const double z = 2 + x * 0.3f;

    return QVector3D(x, y, z);
}

void SensorSpace::SensorModel::setEnabledSensor(const quint32 &newValue)
{
    m_enabledSensor = newValue;
    calculateStep();
}

void SensorSpace::SensorModel::setAntennaLenght(const double &newLenght)
{
    m_len = newLenght;
    calculateStep();
}

void SensorSpace::SensorModel::setMaxDeviation(const double &newMaxValue)
{

}

double SensorSpace::SensorModel::getStep() const
{
    return m_step;
}

double SensorSpace::SensorModel::getLenght() const
{
    return m_len;
}

quint32 SensorSpace::SensorModel::getEnabledSensor() const
{
    return m_enabledSensor;
}

void SensorSpace::SensorModel::calculateStep()
{
    m_step = m_len / static_cast<double>(m_enabledSensor);
}
