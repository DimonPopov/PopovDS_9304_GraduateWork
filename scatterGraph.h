#ifndef ScatterGraph_H
#define ScatterGraph_H

#include <QWidget>
#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSharedPointer>

#include "abstractpointcontainer.h"



class ScatterGraph : public QWidget
{
    Q_OBJECT
public:
    explicit ScatterGraph(Q3DScatter*,
                          QSharedPointer<PointContainerSpace::PositionSensors>,
                          QSharedPointer<PointContainerSpace::AcousticSensors>,
                          QSharedPointer<PointContainerSpace::TrueModel>);
public slots:
    void handleSetPositionSensorColor(const QColor& newColor);
    void handleSetPositionSensorSize(const double& newValue);
    void handleSetPositionSensorVisibility(const bool& newState);
    void handleSetAcousticSensorColor(const QColor& newColor);
    void handleSetAcousticSensorSize(const double& newValue);
    void handleSetAcousticSensorVisibility(const bool& newState);
    void handleSetTrueModelVisibility(const bool& newState);
    void handleSetTrueModelColor(const QColor& newColor);
    void handleSetTrueModelSize(const double& newValue);

private:
    Q3DScatter* m_graph;
    QSharedPointer<PointContainerSpace::PositionSensors> m_positionSensors;
    QSharedPointer<PointContainerSpace::AcousticSensors> m_acousticSensors;
    QSharedPointer<PointContainerSpace::TrueModel> m_trueModel;
    QScatter3DSeries *m_acousticSensorSeries;
    QScatter3DSeries *m_positionSensorSeries;
    QScatter3DSeries *m_trueAntennaModelSeries;

    void setAxisXRange(const double min, const double max);
    void setAxisZRange(const double min, const double max);
    void setAxisYRange(const double min, const double max);

signals:
    void sigInterpolationTimeUpdate(const quint32& time);

private slots:
    void handleUpdatePositionSensors();
    void handleUpdateAcousticSensors();
    void handleUpdateTrueModel();
    void handleSelectItemChanged(const int& index);
};

#endif // ScatterGraph_H
