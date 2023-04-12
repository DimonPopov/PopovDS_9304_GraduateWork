#include "scatterGraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>
#include <vector>



using namespace PointContainerSpace;

ScatterGraph::ScatterGraph(Q3DScatter *surface,
                           QSharedPointer<PositionSensors> positionSensors,
                           QSharedPointer<AcousticSensors> acousticSensors,
                           QSharedPointer<TrueModel> trueModel)
    : m_graph(surface),
      m_positionSensors(positionSensors),
      m_acousticSensors(acousticSensors),
      m_trueModel(trueModel)
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
    m_graph->axisX()->setRange(0.0f, 12.0f);
    m_graph->axisY()->setRange(0.0f, 7.0f);
    m_graph->axisZ()->setRange(0.0f, 7.0f);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    connect(m_positionSensors.data(), &PositionSensors::sigContainerChanged,
            this, &ScatterGraph::handleUpdatePositionSensors, Qt::UniqueConnection);

    connect(m_acousticSensors.data(), &AcousticSensors::sigContainerChanged,
            this, &ScatterGraph::handleUpdateAcousticSensors, Qt::UniqueConnection);

    connect(m_trueModel.data(), &TrueModel::sigContainerChanged,
            this, &ScatterGraph::handleUpdateTrueModel, Qt::UniqueConnection);

    handleUpdatePositionSensors();
    handleUpdateAcousticSensors();
    handleUpdateTrueModel();

    m_graph->addSeries(m_acousticSensorSeries);
    m_graph->addSeries(m_trueAntennaModelSeries);
    m_graph->addSeries(m_positionSensorSeries);
}

void ScatterGraph::setAxisXRange(const double min, const double max)
{
    m_graph->axisX()->setRange(min, max);
}

void ScatterGraph::setAxisZRange(const double min, const double max)
{
    m_graph->axisZ()->setRange(min, max);
}

void ScatterGraph::setAxisYRange(const double min, const double max)
{
    m_graph->axisY()->setRange(min, max);
}

void ScatterGraph::handleUpdatePositionSensors()
{
    auto strongRef = m_positionSensors->getScatterArray().toStrongRef();
    if (strongRef)
        m_positionSensorSeries->dataProxy()->resetArray(strongRef.data());
}

void ScatterGraph::handleUpdateAcousticSensors()
{
    auto strongRef = m_acousticSensors->getScatterArray().toStrongRef();
    if (strongRef)
        m_acousticSensorSeries->dataProxy()->resetArray(strongRef.data());
}

void ScatterGraph::handleUpdateTrueModel()
{
    auto strongRef = m_trueModel->getScatterArray().toStrongRef();
    if (strongRef)
        m_trueAntennaModelSeries->dataProxy()->resetArray(strongRef.data());
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

void ScatterGraph::handleSetAntennaVisibility(const bool &newState)
{
    m_trueAntennaModelSeries->setVisible(newState);
}

void ScatterGraph::handleSetSensorVisibility(const bool &newState)
{
    m_positionSensorSeries->setVisible(newState);
}

void ScatterGraph::handleSetInterpolationVisibility(const bool &newState)
{
    m_acousticSensorSeries->setVisible(newState);
}

void ScatterGraph::handleSetTrueModelColor(const QColor &newColor)
{
    m_trueAntennaModelSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetTrueModelSize(const double &newValue)
{
    m_trueAntennaModelSeries->setItemSize(newValue);
}
