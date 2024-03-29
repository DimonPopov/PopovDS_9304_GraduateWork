#include <random>

#include "antennamodel.h"



AntennaModelSpace::AntennaModel::AntennaModel(const double& lenght,
                                              const double& offset,
                                              const double& step,
                                              const double& interval,
                                              const QPair<double, double>& model,
                                              const QVector3D& maxNoice,
                                              QObject *parent)
    : QObject(parent),
    m_lenght(lenght),
    m_offset(offset),
    m_step(step),
    m_timer(new QTimer),
    m_noise(lenght / step),
    m_coef(model)
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

QVector3D AntennaModelSpace::AntennaModel::getNewPointPosition(const double &x,
                                                               const bool& positionNoise) const
{
    double _x = x + m_offset;
    double _y = getY(_x);
    double _z = getZ(_x);

    static int v = 0;

    if (positionNoise)
    {
        _x += m_noise[v].x();
        _y += m_noise[v].y();
        _z += m_noise[v].z();
        v == m_noise.size() - 1 ? v = 0 : ++v;
    }

    return QVector3D(_x, _y, _z);
}

QVector3D AntennaModelSpace::AntennaModel::getPosition(const double &x) const
{
    double _x = x;
    return QVector3D(_x, getY(_x), getZ(_x));
}

double AntennaModelSpace::AntennaModel::getY(const double &x) const
{
    return m_coef.first + sin(x) * m_coef.second;
}

double AntennaModelSpace::AntennaModel::getZ(const double &x) const
{
    return 2 + x * 0.3f;
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

void AntennaModelSpace::AntennaModel::handleCoefChanged(const QPair<double, double> &coef)
{
    m_coef = coef;
    emit sigLenghtChanged(m_lenght);
}

