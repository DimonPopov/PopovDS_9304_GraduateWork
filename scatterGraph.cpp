#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>
#include <boost/math/interpolators/barycentric_rational.hpp>
#include <vector>



ScatterGraph::ScatterGraph(Q3DScatter *surface)
    : m_graph(surface),
      m_interpolationCount(0)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    m_dataProxy = new QScatterDataProxy();
    m_dataSeries = new QScatter3DSeries(m_dataProxy);

    m_sensorDataProxy = new QScatterDataProxy();
    m_sensorDataSeries = new QScatter3DSeries(m_sensorDataProxy);

    m_sensorModel = new SensorSpace::SensorModel();

    m_sensors.reserve(100);

    for(quint32 i = 0; i <= 100; ++i)
    {
        m_sensors.emplaceBack(new SensorSpace::Sensor(i, m_sensorModel, this));

        connect(m_sensors[i], &SensorSpace::Sensor::sigSensorUpdateData,
                this, &ScatterGraph::handleSetSensorPosition);
    }

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(-1.0f, 12.0f);
    m_graph->axisY()->setRange(-1.0f, 12.0f);
    m_graph->axisZ()->setRange(-1.0f, 12.0f);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    m_graph->addSeries(m_dataSeries);
    m_graph->addSeries(m_sensorDataSeries);
}

ScatterGraph::~ScatterGraph()
{
    delete m_graph;
    delete m_sensorModel;
    for (const auto& s : m_sensors)
        delete s;
}

void ScatterGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void ScatterGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void ScatterGraph::calculateInterpolation()
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    for (auto& p : *m_sensorDataProxy->array())
    {
        x.emplace_back(p.x());
        y.emplace_back(p.y());
        z.emplace_back(p.z());
    }
    auto interpolator = boost::math::interpolators::barycentric_rational<double>(std::move(x), std::move(y));
    double step = m_sensorModel->getLenght() / static_cast<double>(m_interpolationCount);
    QScatterDataArray *data = new QScatterDataArray;
    for (unsigned i = 0; i < m_interpolationCount; ++i)
    {
        double result = interpolator(i * step);
        *data << QVector3D(i * step, result, 2);
    }
    m_dataProxy->resetArray(data);
}

void ScatterGraph::handleSetSensorPosition(const quint32& positionInArray, const QVector3D data)
{
    m_sensorDataProxy->setItem(positionInArray, QScatterDataItem(data));
    calculateInterpolation();
}

void ScatterGraph::handleSetSensorData(const QPair<quint32, double>& newSensorData)
{
    const quint32 curSensCount = m_sensorModel->getEnabledSensor();
    const quint32 newSensCount = newSensorData.first;

    const double curAntennLenght = m_sensorModel->getLenght();
    const double newAntennLenght = newSensorData.second;

    if (curAntennLenght != newAntennLenght)
    {
        m_sensorModel->setAntennaLenght(newAntennLenght);

        if (newAntennLenght > m_graph->axisX()->max() - 0.5f)
            m_graph->axisX()->setRange(m_graph->axisX()->min(), newAntennLenght + 1);

        if (newAntennLenght + 0.5f < m_graph->axisX()->max())
            m_graph->axisX()->setRange(m_graph->axisX()->min(), newAntennLenght + 1);
    }

    if (curSensCount != newSensCount)
    {
        m_sensorModel->setEnabledSensor(newSensCount);

        if (newSensCount > curSensCount)
            for (unsigned i = curSensCount; i < newSensCount; ++i)
                m_sensorDataProxy->addItem(QVector3D(0,0,0));
        else
            m_sensorDataProxy->removeItems(newSensCount, curSensCount - newSensCount);
    }

    for (unsigned i = 0; i < newSensCount; ++i)
        m_sensorDataProxy->setItem(i, m_sensorModel->getNewSensorPosition(i));
}

void ScatterGraph::handleSetInterpolationColor(const QColor &newColor)
{
    m_dataSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetInterpolationSize(const double &newValue)
{
    m_dataSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetSensorColor(const QColor &newColor)
{
    m_sensorDataSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetSensorSize(const double &newValue)
{
    m_sensorDataSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetEmulationState(const bool &state)
{
    const quint32 curSensCount = m_sensorModel->getEnabledSensor();

    for (quint32 i = 0; i < curSensCount; ++i)
        m_sensors[i]->setTimerStatus(state);
}

void ScatterGraph::handleSetInterpolationCount(const quint32 &newValue)
{
    m_interpolationCount = newValue;
}

void ScatterGraph::handleSetMaxDeviation(const double &newMaxValue)
{
    m_sensorModel->setMaxDeviation(newMaxValue);
}

//void ScatterGraph::handleSetAntennaLenght(const double &newLenght)
//{
//    m_sensorModel->setAntennaLenght(newLenght);
//    const quint32 curSensCount = m_sensorModel->getEnabledSensor();

//    if (newLenght > m_graph->axisX()->max() - 0.5f)
//        m_graph->axisX()->setRange(m_graph->axisX()->min(), newLenght + 1);

//    if (newLenght + 0.5f < m_graph->axisX()->max())
//        m_graph->axisX()->setRange(m_graph->axisX()->min(), newLenght + 1);

//    for (quint32 i = 0; i < curSensCount; ++i)
//        m_sensorDataProxy->setItem(i, m_sensorModel->getNewSensorPosition(i));
//}
