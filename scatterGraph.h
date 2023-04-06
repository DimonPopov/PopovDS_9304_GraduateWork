#ifndef ScatterGraph_H
#define ScatterGraph_H

#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSlider>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include <QScopedPointer>

#include "sensor.h"
#include "interpolator.h"


/**
 * @brief Класс-обертка для Q3DScatter.
 */
class ScatterGraph : public QObject
{
    Q_OBJECT
public:
    explicit ScatterGraph(Q3DScatter* surface,
                          SensorSpace::Sensors* sensors,
                          InterpolatorSpace::Interpolator* interpolator);
    void calculateInterpolation();

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

private:
    QSharedPointer<Q3DScatter> m_graph;                             ///< График для отображеня.
    QScopedPointer<SensorSpace::Sensors> m_positionSensors;         ///< Представление набора сенсоров.
    QScopedPointer<InterpolatorSpace::Interpolator> m_interpolator; ///< Интерполятор.
    QScatter3DSeries *m_acousticSensorSeries;                       ///< Набор востановленных координат акустических сенсоров.
    QScatter3DSeries *m_positionSensorSeries;                       ///< Набор координат сенсоров местоположения.
    QScatter3DSeries *m_trueAntennaModelSeries;                     ///< Набор координат истинной формы антены.

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void updatePositionSensors();
};

#endif // ScatterGraph_H
