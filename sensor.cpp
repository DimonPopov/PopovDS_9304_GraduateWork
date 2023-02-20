#include "sensor.h"



SensorSpace::Sensor::Sensor(QObject* parent)
    : QTimer(parent)
{
    setInterval(20);
}
