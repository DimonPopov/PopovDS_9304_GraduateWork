#ifndef SENSOR
#define SENSOR

#include <QObject>
#include <QVector3D>


class QTimer;

namespace SensorSpace {

class SensorModel
{
public:
    SensorModel(const quint32& sensorCount, const quint32& enabledSensor);
    QVector3D getNewSensorPosition(const quint32& positionInArray, const bool& timerEnable = false);
    void setEnabledSensor(const quint32& newValue);
private:
    const quint32 m_sensorCount;
    quint32 m_enabledSensor;
    double m_step;
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
