#include <random>

#include "antennamodel.h"



AntennaModelSpace::AntennaModel::AntennaModel(const double& lenght,
                                              const double& offset,
                                              const double& step,
                                              const double& interval,
                                              const QVector3D& maxNoice,
                                              QObject *parent)
    : QObject(parent),
    m_lenght(lenght),
    m_offset(offset),
    m_step(step),
    m_timer(new QTimer),
    m_noise(16)
{
    m_timer->setInterval(interval * 1'000);

    connect(m_timer.data(), &QTimer::timeout,
            this, [&](){
                    static double _step = 0.0;
                    if (_step > 2 * M_PI)
                        _step = 0;
                    else
                        _step += m_step;

                    m_offset = _step;

                    emit sigLenghtChanged(m_lenght);
});

    if (!maxNoice.isNull())
        handleMaxNoiseChanged(maxNoice);
}

double AntennaModelSpace::AntennaModel::getLenght() const
{
    return m_lenght;
}

double AntennaModelSpace::AntennaModel::getOffest() const
{
    return m_offset;
}

void AntennaModelSpace::AntennaModel::setLenght(const double& newLenght)
{
    m_lenght = newLenght;
    emit sigLenghtChanged(m_lenght);
}

void AntennaModelSpace::AntennaModel::setOffest(const double& offset)
{
    m_offset = offset;
}

QVector3D AntennaModelSpace::AntennaModel::getNewPointPosition(const double &step,
                                                               const quint32 &pointNumber,
                                                               const NoiseType& type) const
{
    double x = step * pointNumber;
    if (m_timer->isActive()) x += m_offset;
    double y = 3 + sin(2 * x + 0.5f);
    double z = 2 + x * 0.3f;

    static int v = 0;

    if (type == NoiseType::Position)
    {
        x += m_noise[v].x();
        y += m_noise[v].y();
        z += m_noise[v].z();
        v == m_noise.size() - 1 ? v = 0 : ++v;
    }

    return QVector3D(x, y, z);
}

double AntennaModelSpace::AntennaModel::getInterval() const
{
    return m_timer->interval();
}

void AntennaModelSpace::AntennaModel::setInterval(const double &newInterval)
{
    m_timer->setInterval(newInterval * 1'000);
}

double AntennaModelSpace::AntennaModel::getStep() const
{
    return m_step;
}

void AntennaModelSpace::AntennaModel::setStep(const double &newStep)
{
    m_step = newStep;
}

void AntennaModelSpace::AntennaModel::handleMaxNoiseChanged(const QVector3D &maxNoise)
{
    std::default_random_engine rnd {std::random_device{}()};
    std::uniform_real_distribution<double> forX(-maxNoise.x(), maxNoise.x());
    std::uniform_real_distribution<double> forY(-maxNoise.y(), maxNoise.y());
    std::uniform_real_distribution<double> forZ(-maxNoise.z(), maxNoise.z());

    for (unsigned i = 0; i < m_noise.size() - 1; ++i)
        m_noise[i] = QVector3D(forX(rnd), forY(rnd), forZ(rnd));
}

void AntennaModelSpace::AntennaModel::handleStartEmulate(const bool &state)
{
    state ? m_timer->start() : m_timer->stop();
}

