#ifndef ABSTRACTPOINTCONTAINER_H
#define ABSTRACTPOINTCONTAINER_H

#include <QObject>
#include <QScatterDataProxy>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QTimer>

#include "antennamodel.h"



namespace PointContainerSpace {



class AbstractPointContainer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPointContainer(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                                    QObject *parent = nullptr);
    virtual ~AbstractPointContainer();
    QWeakPointer<QScatterDataArray> getScatterArray() const;
    QWeakPointer<AntennaModelSpace::AntennaModel> getAntennaModel() const;
    quint32 getScatterArraySize() const;
    void setScatterArraySize(const quint32& newSize);
protected:
    virtual void updatePointPosition() = 0;
    QSharedPointer<AntennaModelSpace::AntennaModel> m_model;
    QSharedPointer<QScatterDataArray> m_scatterArray;
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
public:
    explicit PositionSensors(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                             const quint32& amountPoints,
                             QObject *parent = nullptr);
protected:
    void updatePointPosition() override;
    QSharedPointer<QTimer> m_timer;
};



class AcousticSensors : public AbstractPointContainer
{
public:
    explicit AcousticSensors(QSharedPointer<AntennaModelSpace::AntennaModel> model,
                             QSharedPointer<PositionSensors> positionSensors,
                             const quint32& amountPoints,
                             QObject *parent = nullptr);
protected:
    void updatePointPosition() override;
    QWeakPointer<PositionSensors> m_positionSensors;
};

}



#endif // ABSTRACTPOINTCONTAINER_H
