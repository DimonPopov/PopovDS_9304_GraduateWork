#ifndef SENSOR
#define SENSOR

#include <QObject>
#include <QVector3D>


class QTimer;

namespace SensorSpace {

class SensorModel
{
public:
    SensorModel(const quint32& enabledSensor = 0);
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
    Sensor(const quint32 position, SensorModel* model, QObject* parent = nullptr);
    void setTimerStatus(bool status);
private:
    const quint32 m_positionInArray;
    SensorModel* m_model;
    QTimer* m_updateDataTimer;
    QVector3D m_currentPosition;
signals:
    void sigSensorUpdateData(const quint32& positionInArray, const QVector3D data);
private slots:
    void handleTimerTimeout();
};

}



#endif // SENSOR
