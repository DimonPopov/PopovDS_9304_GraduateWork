#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>



ScatterGraph::ScatterGraph(Q3DScatter *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    m_dataProxy = new QScatterDataProxy();
    m_dataSeries = new QScatter3DSeries(m_dataProxy);

    m_dataSeries->setItemSize(0.3);

    m_sensorDataProxy = new QScatterDataProxy();
    m_sensorDataSeries = new QScatter3DSeries(m_sensorDataProxy);

    m_sensorDataSeries->setBaseColor("red");
    m_sensorDataSeries->setItemSize(0.4);

    m_sensors.resize(99);
    m_sensors.shrink_to_fit();

    for (int i = 0; i < 99; ++i)
    {
        m_sensors[i] = new SensorSpace::Sensor(i, this);

        connect(m_sensors[i], &SensorSpace::Sensor::sigSensorUpdateData,
                this, &ScatterGraph::handleAddSensorPoint);
    }

    initTestData();

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

    m_enabledSensors = 3;
    for (unsigned i = 0; i < m_enabledSensors; ++i)
        m_sensors[i]->setTimerStatus(true);
}

ScatterGraph::~ScatterGraph()
{
    delete m_graph;
    for (const auto& s : m_sensors)
        delete s;
}

void ScatterGraph::initTestData()
{
    QScatterDataArray *data = new QScatterDataArray;
    *data << QVector3D(0.0f, 0.0f, 0.0f)
          << QVector3D(1.1f, 0.0f, 0.0f)
          << QVector3D(2.2f, 0.0f, 0.0f)
          << QVector3D(3.3f, 0.0f, 0.0f)
          << QVector3D(4.4f, 0.0f, 0.0f)
          << QVector3D(5.5f, 0.0f, 0.0f)
          << QVector3D(6.6f, 0.0f, 0.0f);

    QScatterDataArray *dataForSensor = new QScatterDataArray;
    *dataForSensor << QVector3D(0.0f, 2.0f, 0.0f)
                   << QVector3D(1.0f, 2.0f, 0.0f)
                   << QVector3D(2.0f, 2.0f, 0.0f);

    m_dataProxy->resetArray(data);
    m_sensorDataProxy->resetArray(dataForSensor);
}

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
    else if (newValue > m_enabledSensors)
        for (unsigned i = m_enabledSensors; i < newValue; ++i)
        {
            m_sensorDataProxy->addItem(QVector3D(12,12,12));
            m_sensors[i]->setTimerStatus(true);
        }
    else
    {
        for (unsigned i = newValue; i < m_enabledSensors; ++i)
            m_sensors[i]->setTimerStatus(false);
        m_sensorDataProxy->removeItems(newValue, m_enabledSensors - newValue);
    }
    qDebug() << m_enabledSensors << newValue << m_sensorDataProxy->array()->size();
}

void ScatterGraph::handleAddSensorPoint(const quint32& positionInArray, const QVector3D data)
{
    m_sensorDataProxy->setItem(positionInArray, QScatterDataItem(data));
}

void ScatterGraph::handleSetSensorCount(const quint32 &newValue)
{
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

void ScatterGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void ScatterGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


