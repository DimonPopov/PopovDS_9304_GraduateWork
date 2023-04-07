#ifndef ScatterGraph_H
#define ScatterGraph_H

#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSlider>
#include <QVector>
#include <QSharedPointer>
#include <QScopedPointer>

#include "abstractpointcontainer.h"


/**
 * @brief Класс-обертка для Q3DScatter.
 */
class ScatterGraph : public QObject
{
    Q_OBJECT
public:
    explicit ScatterGraph(Q3DScatter* surface,
                          QSharedPointer<PointContainerSpace::PositionSensors>,
                          QSharedPointer<PointContainerSpace::AcousticSensors>,
                          QSharedPointer<PointContainerSpace::TrueModel>);
public slots:
    void handleSetSensorColor(const QColor& newColor);
    void handleSetSensorSize(const double& newValue);
    void handleSetInterpolationColor(const QColor& newColor);
    void handleSetInterpolationSize(const double& newValue);
    void handleSetEmulationState(const bool& state);
    void handleSetMaxDeviation(const double& newMaxValue);
    void handleSetAntennaVisibility(const bool& newState);
    void handleSetSensorVisibility(const bool& newState);
    void handleSetInterpolationVisibility(const bool& newState);
    void handleSetTrueModelColor(const QColor& newColor);
    void handleSetTrueModelSize(const double& newValue);

private:
    QScopedPointer<Q3DScatter> m_graph;                             ///< График для отображеня.
    QSharedPointer<PointContainerSpace::PositionSensors> m_positionSensors;
    QSharedPointer<PointContainerSpace::AcousticSensors> m_acousticSensors;
    QSharedPointer<PointContainerSpace::TrueModel> m_trueModel;
    QScatter3DSeries *m_acousticSensorSeries;                       ///< Набор востановленных координат акустических сенсоров.
    QScatter3DSeries *m_positionSensorSeries;                       ///< Набор координат сенсоров местоположения.
    QScatter3DSeries *m_trueAntennaModelSeries;                     ///< Набор координат истинной формы антены.

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

private slots:
    void handleUpdatePositionSensors();
    void handleUpdateAcousticSensors();
    void handleUpdateTrueModel();
};

#endif // ScatterGraph_H
