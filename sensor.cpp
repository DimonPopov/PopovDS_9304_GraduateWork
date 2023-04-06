#include <QTimer>
#include <qmath.h>
#include <QDebug>

#include "sensor.h"


SensorSpace::Sensor::Sensor(const quint32& position, QObject* parent)
    : QObject(parent),
      m_sensorNumber(position),
      m_timer(new QTimer(this))
{
    m_timer->setInterval(1'000);
}

void SensorSpace::Sensor::setTimerStatus(bool status)
{
    if (m_timer)
        status ? m_timer->start() : m_timer->stop();
}

void SensorSpace::Sensor::handleTimerTimeout()
{
    emit sigSensorTriggered(m_sensorNumber);
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
    Q_UNUSED(newMaxValue);
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

SensorSpace::Sensors::Sensors(const quint32 &sensorCount, const double &modelLenght, QObject* parent)
    : QObject(parent),
      m_model(new SensorModel(sensorCount, modelLenght))
{
    m_sensors.reserve(sensorCount);
    for (unsigned i = 0; i < sensorCount; ++i)
    {
        Sensor* newSensor = new Sensor(i, this);
        m_sensors.push_back(newSensor);

        connect(newSensor, &SensorSpace::Sensor::sigSensorTriggered,
                this, &SensorSpace::Sensors::handleSensorTrigger);
    }
}

quint32 SensorSpace::Sensors::getSensorCount() const
{
    return m_sensors.size();
}

double SensorSpace::Sensors::getModelLenght() const
{
    return m_model->getLenght();
}

QVector3D SensorSpace::Sensors::getSensorPosition(const quint32 &index) const
{
    return m_model->getNewSensorPosition(index);
}

SensorSpace::Sensor *SensorSpace::Sensors::operator[](const quint32 &index) const
{
    return index >= getSensorCount() ? nullptr : m_sensors[index];
}

void SensorSpace::Sensors::handleSensorTrigger(const quint32 &sensorNumber)
{
    emit sigUpdateSensorPosition(sensorNumber, m_model->getNewSensorPosition(sensorNumber));
}

void SensorSpace::Sensors::handleSetSensorCount(const quint32 &newSensorCount)
{
    const quint32 curSensorCount = getSensorCount();

    if (newSensorCount == curSensorCount)
        return;

    m_model->setEnabledSensor(newSensorCount);

    if (newSensorCount > curSensorCount)
    {
        for (unsigned i = curSensorCount; i < newSensorCount; ++i)
        {
            auto newSensor = new SensorSpace::Sensor(i, this);

            connect(newSensor, &SensorSpace::Sensor::sigSensorTriggered,
                    this, &SensorSpace::Sensors::handleSensorTrigger);

            m_sensors.push_back(newSensor);
        }
    }
    else
    {
        for (unsigned i = curSensorCount; i > newSensorCount; --i)
        {
            delete m_sensors[i - 1];
            m_sensors.pop_back();
        }
    }

    emit sigModelChanged();
}

void SensorSpace::Sensors::handleSetModelLenght(const double &newModelLenght)
{
    m_model->setAntennaLenght(newModelLenght);

    emit sigModelChanged();
}
