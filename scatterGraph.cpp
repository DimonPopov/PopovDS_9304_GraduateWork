#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>
#include <boost/math/interpolators/barycentric_rational.hpp>
#include <vector>
#include <boost/rational.hpp>



ScatterGraph::ScatterGraph(Q3DScatter *surface)
    : m_graph(surface),
      m_enabledSensors(0),
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

    m_sensorModel = new SensorSpace::SensorModel(99, m_enabledSensors);

    m_sensors.resize(99);
    m_sensors.shrink_to_fit();

    for (int i = 0; i < 99; ++i)
    {
        m_sensors[i] = new SensorSpace::Sensor(i, m_sensorModel, this);

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

//void ScatterGraph::initTestData()
//{
//    QScatterDataArray *data = new QScatterDataArray;
//    *data << QVector3D(0.0f, 0.0f, 0.0f)
//          << QVector3D(1.1f, 0.0f, 0.0f)
//          << QVector3D(2.2f, 0.0f, 0.0f)
//          << QVector3D(3.3f, 0.0f, 0.0f)
//          << QVector3D(4.4f, 0.0f, 0.0f)
//          << QVector3D(5.5f, 0.0f, 0.0f)
//          << QVector3D(6.6f, 0.0f, 0.0f);

//    QScatterDataArray *dataForSensor = new QScatterDataArray;
//    *dataForSensor << QVector3D(0.0f, 2.0f, 0.0f)
//                   << QVector3D(1.0f, 2.0f, 0.0f)
//                   << QVector3D(2.0f, 2.0f, 0.0f);

//    m_dataProxy->resetArray(data);
//    m_sensorDataProxy->resetArray(dataForSensor);
//}

void ScatterGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void ScatterGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void ScatterGraph::updateSensorArray(const quint32& newValue)
{
    if (newValue == m_enabledSensors)
        return;

    if (newValue > m_enabledSensors)
        for (unsigned i = m_enabledSensors; i < newValue; ++i)
            m_sensorDataProxy->addItem(QVector3D(0,0,0));
    else
        m_sensorDataProxy->removeItems(newValue, m_enabledSensors - newValue);

    for (unsigned i = 0; i < newValue; ++i)
        m_sensorDataProxy->setItem(i, m_sensorModel->getNewSensorPosition(i));
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
    double step = 10.0f / static_cast<double>(m_interpolationCount);
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

void ScatterGraph::handleSetSensorCount(const quint32 &newValue)
{
    m_sensorModel->setEnabledSensor(newValue);
    updateSensorArray(newValue);
    m_enabledSensors = newValue;
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
    for (unsigned i = 0; i < m_enabledSensors; ++i)
        m_sensors[i]->setTimerStatus(state);
}

void ScatterGraph::handleSetInterpolationCount(const quint32 &newValue)
{
    m_interpolationCount = newValue;
}

//void ScatterGraph::setBlackToYellowGradient()
//{
//    QLinearGradient gr;
//    gr.setColorAt(0.0, Qt::black);
//    gr.setColorAt(0.33, Qt::blue);
//    gr.setColorAt(0.67, Qt::red);
//    gr.setColorAt(1.0, Qt::yellow);

//    m_graph->seriesList().at(0)->setBaseGradient(gr);
//    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
//}

//void ScatterGraph::setGreenToRedGradient()
//{
//    QLinearGradient gr;
//    gr.setColorAt(0.0, Qt::darkGreen);
//    gr.setColorAt(0.5, Qt::yellow);
//    gr.setColorAt(0.8, Qt::red);
//    gr.setColorAt(1.0, Qt::darkRed);

//    m_graph->seriesList().at(0)->setBaseGradient(gr);
//    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
//}


