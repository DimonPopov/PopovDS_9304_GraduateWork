#ifndef ScatterGraph_H
#define ScatterGraph_H

#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSlider>
#include <QVector>

#include "sensor.h"



class ScatterGraph : public QObject
{
    Q_OBJECT
public:
    explicit ScatterGraph(Q3DScatter *surface);
    ~ScatterGraph();

//    void setBlackToYellowGradient();
//    void setGreenToRedGradient();

public slots:
    void handleSetSensorPosition(const quint32& positionInArray, const QVector3D data);
    void handleSetSensorCount(const quint32& newValue);
    void handleSetSensorColor(const QColor& newColor);
    void handleSetSensorSize(const double& newValue);
    void handleSetInterpolationColor(const QColor& newColor);
    void handleSetInterpolationSize(const double& newValue);
    void handleSetEmulationState(const bool& state);
    void handleSetInterpolationCount(const quint32& newValue);

private:
    Q3DScatter *m_graph;
    QScatterDataProxy *m_dataProxy;
    QScatter3DSeries *m_dataSeries;
    QScatterDataProxy *m_sensorDataProxy;
    QScatter3DSeries *m_sensorDataSeries;
    QVector<SensorSpace::Sensor*> m_sensors;
    SensorSpace::SensorModel* m_sensorModel;
    quint32 m_enabledSensors;
    quint32 m_interpolationCount;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void updateSensorArray(const quint32& newValue);
    void calculateInterpolation();
    //    void initTestData();
};

#endif // ScatterGraph_H
