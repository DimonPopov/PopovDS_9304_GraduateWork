#ifndef SENSOR
#define SENSOR

#include <QTimer>


namespace SensorSpace {

class Sensor : public QTimer
{
    Q_OBJECT
public:
    Sensor(QObject* parent = nullptr);
};


}



#endif // SENSOR
