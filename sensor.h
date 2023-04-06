#ifndef SENSOR
#define SENSOR

#include <QObject>
#include <QVector3D>
#include <QScopedPointer>



class QTimer;

namespace SensorSpace {

class SensorModel
{
public:
    SensorModel(const quint32& enabledSensor = 0, const double& lenght = 0);
    QVector3D getNewSensorPosition(const quint32& positionInArray, const bool& timerEnable = false);
    void setEnabledSensor(const quint32& newValue);
    void setAntennaLenght(const double& newLenght);
    void setMaxDeviation(const double& newMaxValue);
    double getStep() const;
    double getLenght() const;
    quint32 getEnabledSensor() const;
private:
    void calculateStep();
    quint32 m_enabledSensor;
    double m_step;
    double m_len;
};

class Sensor : public QObject
{
    Q_OBJECT
public:
    Sensor(const quint32& position, QObject* parent = nullptr);
    void setTimerStatus(bool status);
private:
    const quint32 m_sensorNumber;
    QTimer* m_timer;
private slots:
    void handleTimerTimeout();
signals:
    void sigSensorTriggered(const quint32& sensorNumber);
};

class Sensors : public QObject
{
    Q_OBJECT
public:
    explicit Sensors(const quint32& sensorCount, const double &modelLenght, QObject* parent = nullptr);
    quint32 getSensorCount() const;
    double getModelLenght() const;
    QVector3D getSensorPosition(const quint32& index) const;
    Sensor* operator[](const quint32& index) const;
private:
    QScopedPointer<SensorModel> m_model;
    QVector<Sensor*> m_sensors;
public slots:
    void handleSensorTrigger(const quint32& sensorNumber);
    void handleSetSensorCount(const quint32& newSensorCount);
    void handleSetModelLenght(const double& newModelLenght);
signals:
    void sigUpdateSensorPosition(const quint32& sensorNumber, const QVector3D& newPosition);
    void sigModelChanged();
};



}

#endif // SENSOR
