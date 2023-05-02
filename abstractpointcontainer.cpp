#include "abstractpointcontainer.h"



using namespace AntennaModelSpace;

PointContainerSpace::AbstractPointContainer::AbstractPointContainer(QSharedPointer<AntennaModel> model,
                                                                    QObject *parent)
    : QObject(parent),
    m_model(model),
    m_scatterArray(new QScatterDataArray)
{
    connect(model.data(), &AntennaModel::sigLenghtChanged,
            this, &AbstractPointContainer::updatePointPosition);
}

QScatterDataArray* PointContainerSpace::AbstractPointContainer::getScatterArray() const
{
    return m_scatterArray;
}

QWeakPointer<AntennaModelSpace::AntennaModel> PointContainerSpace::AbstractPointContainer::getAntennaModel() const
{
    return m_model;
}

quint32 PointContainerSpace::AbstractPointContainer::getScatterArraySize() const
{
    return m_scatterArray->size();
}

void PointContainerSpace::AbstractPointContainer::setScatterArraySize(const quint32 &newSize)
{
    const quint32 curSize = m_scatterArray->size();

    if (newSize == curSize)
        return;

    if (newSize > curSize)
        for (unsigned i = curSize; i < newSize; ++i)
            m_scatterArray->push_back(QVector3D());
    else
        for (unsigned i = curSize; i > newSize; --i)
            m_scatterArray->pop_back();

    updatePointPosition();
}

PointContainerSpace::PositionSensors::PositionSensors(QSharedPointer<AntennaModel> model,
                                 const quint32 &amountPoints,
                                 const bool& sensorEnd,
                                 const bool& noiseNeed,
                                 QObject *parent)
    : AbstractPointContainer(model, parent),
    m_sensorEnd(sensorEnd),
    m_needNoise(noiseNeed)
{
    setScatterArraySize(amountPoints);
}

void PointContainerSpace::PositionSensors::updatePointPosition()
{
    const quint32 size = m_scatterArray->size();
    const double step = m_model->getLenght() / (m_sensorEnd ? size - 1 : size);

    m_scatterArray->clear();

    for (unsigned i = 0; i < size; ++i)
        *m_scatterArray << m_model->getNewPointPosition(step * i, m_needNoise ? true : false);

    emit sigContainerChanged();
}

void PointContainerSpace::PositionSensors::handleSetSensorEnd(const bool &state)
{
    m_sensorEnd = state;
    updatePointPosition();
}

void PointContainerSpace::PositionSensors::handleSetNeedNoise(const bool &state)
{
    m_needNoise = state;
    updatePointPosition();
}



PointContainerSpace::AcousticSensors::AcousticSensors(QSharedPointer<AntennaModel> model,
                                 QSharedPointer<PositionSensors> positionSensors,
                                 const InterpolaionSpace::InterpolationType& type,
                                 const quint32 &amountPoints,
                                 QObject *parent)
    : AbstractPointContainer(model, parent),
    m_сompare(new ErrorComplexityHandler),
    m_positionSensors(positionSensors),
    m_type(type)
{
    disconnect(model.data(), &AntennaModel::sigLenghtChanged,
               this, &AcousticSensors::updatePointPosition);

    connect(positionSensors.data(), &PositionSensors::sigContainerChanged,
            this, &AcousticSensors::updatePointPosition);

    setScatterArraySize(amountPoints);
}

void PointContainerSpace::AcousticSensors::setInterpolationType(const InterpolaionSpace::InterpolationType &newType)
{
    if (newType == m_type)
        return;

    m_type = newType;

//    if (newType == InterpolaionSpace::InterpolationType::CardinalCubicBSpline)
//    {
//        if (auto strongRef = m_positionSensors.toStrongRef())
//            if (strongRef->getScatterArraySize() < 5)
//                strongRef->setScatterArraySize(5);
//    }
//    else
    updatePointPosition();
}

quint32 PointContainerSpace::AcousticSensors::getInterpolationTime() const
{
        return m_сompare->getTime();
}

InterpolaionSpace::InterpolationType PointContainerSpace::AcousticSensors::getInterpolationType() const
{
    return m_type;
}

void PointContainerSpace::AcousticSensors::updatePointPosition()
{
    const quint32 size = m_scatterArray->size();

    m_scatterArray->clear();

    if (auto positionSensorStrongRef = m_positionSensors.toStrongRef())
    {
        m_сompare->startTimer();
        m_scatterArray->append(InterpolaionSpace::calculateInterpolation(positionSensorStrongRef->getScatterArray(),
                                                                         m_model->getLenght(),
                                                                         size,
                                                                         m_type));
        m_сompare->stopTimer();

        emit sigContainerChanged();
    }
}

PointContainerSpace::TrueModel::TrueModel(QSharedPointer<AntennaModel> model,
                                          const quint32 &amountPoints,
                                          QObject *parent)
    : AbstractPointContainer(model, parent)
{
    setScatterArraySize(amountPoints);
}

void PointContainerSpace::TrueModel::updatePointPosition()
{
    const quint32 size = m_scatterArray->size();
    const double step = m_model->getLenght() / size;

    m_scatterArray->clear();

    for (unsigned i = 0; i < size; ++i)
        *m_scatterArray << m_model->getNewPointPosition(step * i);

    emit sigContainerChanged();
}
