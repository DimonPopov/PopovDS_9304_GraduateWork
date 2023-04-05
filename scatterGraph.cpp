#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>
#include <boost/math/interpolators/barycentric_rational.hpp>
#include <vector>



ScatterGraph::ScatterGraph(Q3DScatter *surface, SensorSpace::SensorModel* model)
    : m_graph(surface),
      m_sensorModel(model),
      m_interpolationCount(0)
{
    // Инициализация сетки графика.

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    m_interpolationSeries = new QScatter3DSeries(new QScatterDataProxy);
    m_antennaSeries       = new QScatter3DSeries(new QScatterDataProxy);
    m_sensorSeries        = new QScatter3DSeries(new QScatterDataProxy);

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

    // Инициализация сенсоров, добавление их местоположения на график.

    for (unsigned i = 0; i < m_sensorModel->getEnabledSensor(); ++i)
    {
        auto newSensor = QSharedPointer<SensorSpace::Sensor>(new SensorSpace::Sensor(i, m_sensorModel, this));

        connect(newSensor.data(), &SensorSpace::Sensor::sigSensorUpdateData,
                this, &ScatterGraph::handleSetSensorPosition);

        m_sensors.push_back(newSensor);
        m_sensorSeries->dataProxy()->addItem(m_sensorModel->getNewSensorPosition(i));
    }

    m_graph->addSeries(m_interpolationSeries);
    m_graph->addSeries(m_antennaSeries);
    m_graph->addSeries(m_sensorSeries);
}

void ScatterGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void ScatterGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void ScatterGraph::updateSens(const quint32 &newCount)
{
    const quint32 value = (newCount == 0 ? m_sensorModel->getEnabledSensor() : newCount);

    for (unsigned i = 0; i < value; ++i)
        m_sensorSeries->dataProxy()->setItem(i, m_sensorModel->getNewSensorPosition(i));

    calculateInterpolation();
}

void ScatterGraph::calculateInterpolation()
{
    std::vector<double> x;
    std::vector<double> x2;
    std::vector<double> y;
    std::vector<double> z;

    for (auto& p : *m_sensorSeries->dataProxy()->array())
    {
        x.emplace_back(p.x());
        x2.emplace_back(p.x());
        y.emplace_back(p.y());
        z.emplace_back(p.z());
    }

    auto interpolatorX = boost::math::interpolators::barycentric_rational<double>(std::move(x), std::move(y));
    auto interpolatorZ = boost::math::interpolators::barycentric_rational<double>(std::move(x2), std::move(z));

    double step = m_sensorModel->getLenght() / static_cast<double>(m_interpolationCount);
    QScatterDataArray *data = new QScatterDataArray;

    for (unsigned i = 0; i < m_interpolationCount; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        *data << QVector3D(i * step, Y, Z);
    }

    m_interpolationSeries->dataProxy()->resetArray(data);
}

void ScatterGraph::handleSetSensorPosition(const quint32& positionInArray, const QVector3D data)
{
    m_sensorSeries->dataProxy()->setItem(positionInArray, QScatterDataItem(data));
    calculateInterpolation();
}

void ScatterGraph::handleSensorCountChanged(const quint32 &newCount)
{
    const quint32 curSensCount = m_sensorModel->getEnabledSensor();
    const quint32 newSensCount = newCount;

    if (curSensCount == newSensCount)
        return;

    auto proxy = m_sensorSeries->dataProxy();

    m_sensorModel->setEnabledSensor(newSensCount);

    if (newSensCount > curSensCount)
        for (unsigned i = curSensCount; i < newSensCount; ++i)
        {
            auto newSensor = QSharedPointer<SensorSpace::Sensor>(new SensorSpace::Sensor(i, m_sensorModel, this));

            connect(newSensor.data(), &SensorSpace::Sensor::sigSensorUpdateData,
                    this, &ScatterGraph::handleSetSensorPosition);

            m_sensors.push_back(newSensor);
            m_sensorSeries->dataProxy()->addItem(m_sensorModel->getNewSensorPosition(i));
        }
    else
    {
        for (unsigned i = curSensCount; i > newSensCount; --i)
            m_sensors.pop_back();

        proxy->removeItems(newSensCount, curSensCount - newSensCount);
    }

    updateSens(curSensCount);
}

void ScatterGraph::handleAntennaLenghtChanged(const double &newLenght)
{
    const double curAntennLenght = m_sensorModel->getLenght();
    const double newAntennLenght = newLenght;

    if (curAntennLenght == newAntennLenght)
        return;

    m_sensorModel->setAntennaLenght(newAntennLenght);

    if (newAntennLenght > m_graph->axisX()->max() - 0.5f)
        m_graph->axisX()->setRange(m_graph->axisX()->min(), newAntennLenght + 1);

    if (newAntennLenght + 0.5f < m_graph->axisX()->max())
        m_graph->axisX()->setRange(m_graph->axisX()->min(), newAntennLenght + 1);

    updateSens();
}

void ScatterGraph::handleSetInterpolationColor(const QColor &newColor)
{
    m_interpolationSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetInterpolationSize(const double &newValue)
{
    m_interpolationSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetSensorColor(const QColor &newColor)
{
    m_sensorSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetSensorSize(const double &newValue)
{
    m_sensorSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetEmulationState(const bool &state)
{
    for (const auto& s : m_sensors)
        s->setTimerStatus(state);
}

void ScatterGraph::handleSetInterpolationCount(const quint32 &newValue)
{
    m_interpolationCount = newValue;
    calculateInterpolation();
}

void ScatterGraph::handleSetMaxDeviation(const double &newMaxValue)
{
    m_sensorModel->setMaxDeviation(newMaxValue);
}

void ScatterGraph::handleSetAntennaVisibility(const bool &newState)
{
    qDebug() << "Antenna = " <<  newState;
}

void ScatterGraph::handleSetSensorVisibility(const bool &newState)
{
    m_sensorSeries->setVisible(newState);
}

void ScatterGraph::handleSetInterpolationVisibility(const bool &newState)
{
    m_interpolationSeries->setVisible(newState);
}
