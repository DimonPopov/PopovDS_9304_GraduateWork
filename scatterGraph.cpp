#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>
#include <vector>



ScatterGraph::ScatterGraph(Q3DScatter *surface, SensorSpace::Sensors* sensors, InterpolatorSpace::Interpolator* interpolator)
    : m_graph(surface),
      m_positionSensors(sensors),
      m_interpolator(interpolator)
{
    // Инициализация сетки графика.

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    m_acousticSensorSeries   = new QScatter3DSeries(new QScatterDataProxy);
    m_trueAntennaModelSeries = new QScatter3DSeries(new QScatterDataProxy);
    m_positionSensorSeries   = new QScatter3DSeries(new QScatterDataProxy);

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(0.0f, sensors->getModelLenght() + 1.0f);
    m_graph->axisY()->setRange(0.0f, 12.0f);
    m_graph->axisZ()->setRange(0.0f, 12.0f);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    // Добавление сенсоров на график.

    for (unsigned i = 0; i < m_positionSensors->getSensorCount(); ++i)
        m_positionSensorSeries->dataProxy()->addItem(m_positionSensors->getSensorPosition(i));

    // При изменении модели антенны (длины\кол-ва датчиков) обновить визуал графика.

    connect(sensors, &SensorSpace::Sensors::sigModelChanged,
            this, &ScatterGraph::updatePositionSensors);

    connect(interpolator, &InterpolatorSpace::Interpolator::sigInterpolatorChanged,
            this, &ScatterGraph::calculateInterpolation);

    m_graph->addSeries(m_acousticSensorSeries);
    m_graph->addSeries(m_trueAntennaModelSeries);
    m_graph->addSeries(m_positionSensorSeries);

    calculateInterpolation();
}

void ScatterGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void ScatterGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void ScatterGraph::updatePositionSensors()
{
    auto newArray = new QScatterDataArray;

    for (unsigned i = 0; i < m_positionSensors->getSensorCount(); ++i)
        *newArray << m_positionSensors->getSensorPosition(i);

    m_positionSensorSeries->dataProxy()->resetArray(newArray);

    calculateInterpolation();
}

void ScatterGraph::calculateInterpolation()
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;

    for (auto& p : *m_positionSensorSeries->dataProxy()->array())
    {
        x.push_back(p.x());
        y.push_back(p.y());
        z.push_back(p.z());
    }

    auto list = m_interpolator->calculateInterpolation(x, y, z);
    auto data = new QScatterDataArray;

    for (const auto& l : list)
        *data << l;

    m_acousticSensorSeries->dataProxy()->resetArray(data);
}

void ScatterGraph::handleSetInterpolationColor(const QColor &newColor)
{
    m_acousticSensorSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetInterpolationSize(const double &newValue)
{
    m_acousticSensorSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetSensorColor(const QColor &newColor)
{
    m_positionSensorSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetSensorSize(const double &newValue)
{
    m_positionSensorSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetEmulationState(const bool &state)
{
//    for (const auto& s : m_positionSensors)
//        s->setTimerStatus(state);
}

void ScatterGraph::handleSetMaxDeviation(const double &newMaxValue)
{
//    m_sensorModel->setMaxDeviation(newMaxValue);
}

void ScatterGraph::handleSetAntennaVisibility(const bool &newState)
{
    qDebug() << "Antenna = " <<  newState;
}

void ScatterGraph::handleSetSensorVisibility(const bool &newState)
{
    m_positionSensorSeries->setVisible(newState);
}

void ScatterGraph::handleSetInterpolationVisibility(const bool &newState)
{
    m_acousticSensorSeries->setVisible(newState);
}
