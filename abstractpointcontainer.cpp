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

PointContainerSpace::AbstractPointContainer::~AbstractPointContainer()
{

}

QWeakPointer<QScatterDataArray> PointContainerSpace::AbstractPointContainer::getScatterArray() const
{
    return m_scatterArray.toWeakRef();
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
                                 QObject *parent)
    : AbstractPointContainer(model, parent),
      m_timer(new QTimer)
{
    m_timer->setInterval(1'000);

    connect(m_timer.data(), &QTimer::timeout,
            this, &PositionSensors::updatePointPosition);

    setScatterArraySize(amountPoints);
}

void PointContainerSpace::PositionSensors::updatePointPosition()
{
    const quint32 size = m_scatterArray->size();
    const double step = m_model->getLenght() / size;

    m_scatterArray->clear();

    for (unsigned i = 0; i < size; ++i)
        *m_scatterArray << m_model->getNewPointPosition(step, i);

    emit sigContainerChanged();
}

void PointContainerSpace::PositionSensors::handleSetNoiseState(const bool &noise)
{
    noise ? m_timer->start() : m_timer->stop();
}



PointContainerSpace::AcousticSensors::AcousticSensors(QSharedPointer<AntennaModel> model,
                                 QSharedPointer<PositionSensors> positionSensors,
                                 const InterpolaionSpace::InterpolationType& type,
                                 const quint32 &amountPoints,
                                 QObject *parent)
    : AbstractPointContainer(model, parent),
      m_positionSensors(positionSensors),
      m_type(type)
{
    connect(positionSensors.data(), &PositionSensors::sigContainerChanged,
            this, &AcousticSensors::updatePointPosition);

    setScatterArraySize(amountPoints);
}

void PointContainerSpace::AcousticSensors::setInterpolationType(const InterpolaionSpace::InterpolationType &newType)
{
    if (newType == m_type)
        return;

    m_type = newType;
    updatePointPosition();
}

InterpolaionSpace::InterpolationType PointContainerSpace::AcousticSensors::getInterpolationType() const
{
    return m_type;
}

void PointContainerSpace::AcousticSensors::updatePointPosition()
{
    const quint32 size = m_scatterArray->size();

    m_scatterArray->clear();

    auto strongRef = m_positionSensors.toStrongRef();
    if (strongRef)
    {
        auto res = InterpolaionSpace::calculateInterpolation(strongRef->getScatterArray(), m_model->getLenght(), size, m_type);

        for(const auto& p : res)
            *m_scatterArray << p;

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
        *m_scatterArray << m_model->getNewPointPosition(step, i);

    emit sigContainerChanged();
}
