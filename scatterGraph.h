#ifndef ScatterGraph_H
#define ScatterGraph_H

#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSlider>
#include <QVector>
#include <QPair>

#include "sensor.h"



class ScatterGraph : public QObject
{
    Q_OBJECT
public:
    explicit ScatterGraph(Q3DScatter *surface);
    void calculateInterpolation();
    ~ScatterGraph();

public slots:
    void handleSetSensorPosition(const quint32& positionInArray, const QVector3D data);
    void handleSetSensorData(const QPair<quint32, double>& newSensorData);
    void handleSetSensorColor(const QColor& newColor);
    void handleSetSensorSize(const double& newValue);
    void handleSetInterpolationColor(const QColor& newColor);
    void handleSetInterpolationSize(const double& newValue);
    void handleSetEmulationState(const bool& state);
    void handleSetInterpolationCount(const quint32& newValue);
    void handleSetMaxDeviation(const double& newMaxValue);
    void handleSetAntennaVisibility(const bool& newState);
    void handleSetSensorVisibility(const bool& newState);
    void handleSetInterpolationVisibility(const bool& newState);

private:
    Q3DScatter *m_graph;
    QScatter3DSeries *m_interpolationSeries;
    QScatter3DSeries *m_sensorSeries;
    QScatter3DSeries *m_antennaSeries;
    QVector<SensorSpace::Sensor*> m_sensors;
    SensorSpace::SensorModel* m_sensorModel;
    quint32 m_interpolationCount;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

};

#endif // ScatterGraph_H
