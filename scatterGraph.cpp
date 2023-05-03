#include "scatterGraph.h"



using namespace PointContainerSpace;

ScatterGraph::ScatterGraph(Q3DScatter *surface,
                           QSharedPointer<PositionSensors> positionSensors,
                           QSharedPointer<AcousticSensors> acousticSensors,
                           QSharedPointer<TrueModel> trueModel)
    : m_graph(surface),
    m_positionSensors(positionSensors),
    m_acousticSensors(acousticSensors),
    m_trueModel(trueModel),
    showTruePosition(true)
{
    // Инициализация сетки графика.

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    m_acousticSensorSeries   = new QScatter3DSeries(new QScatterDataProxy);
    m_trueAntennaModelSeries = new QScatter3DSeries(new QScatterDataProxy);
    m_positionSensorSeries   = new QScatter3DSeries(new QScatterDataProxy);

    m_acousticSensorSeries->setItemLabelVisible(true);
    m_trueAntennaModelSeries->setItemLabelVisible(true);
    m_positionSensorSeries->setItemLabelVisible(true);

    m_graph->axisX()->setLabelFormat("");
    m_graph->axisY()->setLabelFormat("");
    m_graph->axisZ()->setLabelFormat("");

    m_graph->axisY()->setRange(-3.0f, 7.0f);

    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    connect(m_acousticSensorSeries, &QScatter3DSeries::selectedItemChanged,
            this, &ScatterGraph::handleShowAcousticlItem, Qt::UniqueConnection);

    connect(m_trueAntennaModelSeries, &QScatter3DSeries::selectedItemChanged,
            this, &ScatterGraph::handleShowModelItem, Qt::UniqueConnection);

    connect(m_positionSensorSeries, &QScatter3DSeries::selectedItemChanged,
            this, &ScatterGraph::handleShowPositionItem, Qt::UniqueConnection);

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
    m_positionSensorSeries->dataProxy()->resetArray(m_positionSensors->getScatterArray());
    handleShowPositionItem(m_positionSensorSeries->selectedItem());
}

void ScatterGraph::handleUpdateAcousticSensors()
{
    m_acousticSensorSeries->dataProxy()->resetArray(m_acousticSensors->getScatterArray());
    handleShowAcousticlItem(m_acousticSensorSeries->selectedItem());
    emit sigInterpolationTimeUpdate(m_acousticSensors->getInterpolationTime());
}

void ScatterGraph::handleUpdateTrueModel()
{
    m_trueAntennaModelSeries->dataProxy()->resetArray(m_trueModel->getScatterArray());
    handleShowModelItem(m_trueAntennaModelSeries->selectedItem());
    setAllAxisRange();
}

void ScatterGraph::handleShowModelItem(const int &index)
{
    if (index == -1)
        return;

    auto PP = m_trueAntennaModelSeries->dataProxy()->itemAt(index)->position();
    m_trueAntennaModelSeries->setItemLabelFormat(QString::number(PP.x(), 'f', 3) + ", " +
                                                 QString::number(PP.y(), 'f', 3) + ", " +
                                                 QString::number(PP.z(), 'f', 3));
}

void ScatterGraph::handleShowPositionItem(const int &index)
{
    if (index == -1)
        return;

    auto PP = m_positionSensorSeries->dataProxy()->itemAt(index)->position();
    m_positionSensorSeries->setItemLabelFormat(QString::number(PP.x(), 'f', 3) + ", " +
                                               QString::number(PP.y(), 'f', 3) + ", " +
                                               QString::number(PP.z(), 'f', 3));
}

void ScatterGraph::handleShowAcousticlItem(const int &index)
{
    if (index == -1)
        return;

    if (showTruePosition)
    {
        auto PP = m_acousticSensorSeries->dataProxy()->itemAt(index)->position();
        m_acousticSensorSeries->setItemLabelFormat(QString::number(PP.x(), 'f', 3) + ", " +
                                                   QString::number(PP.y(), 'f', 3) + ", " +
                                                   QString::number(PP.z(), 'f', 3));
    }
    else
    {
        if (auto strongRef = m_acousticSensors->getAntennaModel().toStrongRef())
        {
            QVector3D P = m_acousticSensorSeries->dataProxy()->itemAt(index)->position();
            QVector3D T = strongRef->getPosition(P.x());
            m_acousticSensorSeries->setItemLabelFormat(QString::number(abs(P.x() - T.x()), 'f', 3) + ", " +
                                                       QString::number(abs(P.y() - T.y()), 'f', 3) + ", " +
                                                       QString::number(abs(P.z() - T.z()), 'f', 3));
        }
    }
}

void ScatterGraph::handleSetAcousticSensorColor(const QColor &newColor)
{
    m_acousticSensorSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetAcousticSensorSize(const double &newValue)
{
    m_acousticSensorSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetPositionSensorColor(const QColor &newColor)
{
    m_positionSensorSeries->setBaseColor(newColor);
}

void ScatterGraph::handleSetPositionSensorSize(const double &newValue)
{
    m_positionSensorSeries->setItemSize(newValue);
}

void ScatterGraph::handleSetTrueModelVisibility(const bool &newState)
{
    m_trueAntennaModelSeries->setVisible(newState);
}

void ScatterGraph::handleSetPositionSensorVisibility(const bool &newState)
{
    m_positionSensorSeries->setVisible(newState);
}

void ScatterGraph::handleSetAcousticSensorVisibility(const bool &newState)
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

void ScatterGraph::handleSetDisplayOption(const bool &option)
{
    showTruePosition = option;
    handleShowAcousticlItem(m_acousticSensorSeries->selectedItem());
}

void ScatterGraph::setAllAxisRange()
{
    QVector3D P = m_trueAntennaModelSeries->dataProxy()->itemAt(0)->position();
    QVector3D T = m_trueAntennaModelSeries->dataProxy()->itemAt(m_trueAntennaModelSeries->dataProxy()->array()->size() - 1)->position();
    setAxisXRange(P.x() - 1, T.x() + 1);
    //setAxisYRange(P.y() - 1, T.y() + 1);  // Не прокатит, точки экстемума раскиданы по всему масиву
    setAxisZRange(P.z() - 1, T.z() + 1);
}
