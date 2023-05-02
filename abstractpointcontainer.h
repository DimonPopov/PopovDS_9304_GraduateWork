#ifndef ABSTRACTPOINTCONTAINER_H
#define ABSTRACTPOINTCONTAINER_H

#include <QObject>
#include <QScatterDataProxy>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QTimer>

#include "antennamodel.h"
#include "interpolation.h"
#include "errorcomplexityhandler.h"



namespace PointContainerSpace {



class AbstractPointContainer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPointContainer(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                                    QObject *parent = nullptr);
    virtual ~AbstractPointContainer() {};
    QScatterDataArray* getScatterArray() const;
    QWeakPointer<AntennaModelSpace::AntennaModel> getAntennaModel() const;
    quint32 getScatterArraySize() const;
    void setScatterArraySize(const quint32& newSize);
protected:
    virtual void updatePointPosition() = 0;
    QSharedPointer<AntennaModelSpace::AntennaModel> m_model;
    QScatterDataArray* m_scatterArray;

signals:
    void sigContainerChanged();
};


class TrueModel : public AbstractPointContainer
{
public:
    explicit TrueModel(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                             const quint32& amountPoints,
                             QObject *parent = nullptr);
protected:
    void updatePointPosition() override;
};



class PositionSensors : public AbstractPointContainer
{
    Q_OBJECT
public:
    explicit PositionSensors(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                             const quint32& amountPoints,
                             const bool& sensorEnd,
                             const bool& noiseNeed,
                             QObject *parent = nullptr);
protected:
    void updatePointPosition() override;
    bool m_sensorEnd;
    bool m_needNoise;

public slots:
    void handleSetSensorEnd(const bool& state);
    void handleSetNeedNoise(const bool& state);
};



class AcousticSensors : public AbstractPointContainer
{
public:
    explicit AcousticSensors(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                             QSharedPointer<PositionSensors> positionSensors,
                             const InterpolaionSpace::InterpolationType& newType,
                             const quint32& amountPoints,
                             QObject *parent = nullptr);
    void setInterpolationType(const InterpolaionSpace::InterpolationType& newType);
    quint32 getInterpolationTime() const;
    InterpolaionSpace::InterpolationType getInterpolationType() const;
protected:
    void updatePointPosition() override;
    QScopedPointer<ErrorComplexityHandler> m_сompare;
    QWeakPointer<PositionSensors> m_positionSensors;
    InterpolaionSpace::InterpolationType m_type;
};

}



#endif // ABSTRACTPOINTCONTAINER_H
