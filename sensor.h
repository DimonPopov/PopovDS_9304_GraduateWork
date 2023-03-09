#ifndef SENSOR
#define SENSOR

#include <QObject>
#include <QVector3D>


class QTimer;

namespace SensorSpace {

class Sensor : public QObject
{
    Q_OBJECT
public:
    Sensor(const quint32 position, QObject* parent = nullptr);
    void setTimerStatus(bool status);
private:
    const quint32 m_positionInArray;
    QTimer* m_updateDataTimer;
signals:
    void sigSensorUpdateData(const quint32& positionInArray, const QVector3D data);
private slots:
    void handleTimerTimeout();
};

QVector3D& getSensorPosition(const Sensor* sensor);

}



#endif // SENSOR
