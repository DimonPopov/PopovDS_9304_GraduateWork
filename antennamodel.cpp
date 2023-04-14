#include <random>

#include "antennamodel.h"



AntennaModelSpace::AntennaModel::AntennaModel(const double &lenght,
                                              const QVector3D& maxNoice,
                                              QObject *parent)
    : QObject(parent),
      m_lenght(lenght),
      m_noise(16)
{
    if (!maxNoice.isNull())
        handleMaxNoiseChanged(maxNoice);
}

double AntennaModelSpace::AntennaModel::getLenght() const
{
    return m_lenght;
}

void AntennaModelSpace::AntennaModel::setLenght(double newLenght)
{
    m_lenght = newLenght;
    emit sigLenghtChanged(m_lenght);
}

QVector3D AntennaModelSpace::AntennaModel::getNewPointPosition(const double &step,
                                                               const quint32 &pointNumber,
                                                               const bool& needNoise) const
{
    double x = step * pointNumber;
    double y = 3 + sin(2 * x + 0.5f);
    double z = 2 + x * 0.3f;

    static int v = 0;

    if (needNoise)
    {
        x += m_noise[v].x();
        y += m_noise[v].y();
        z += m_noise[v].z();
        v == m_noise.size() - 1 ? v = 0 : ++v;
    }

    return QVector3D(x, y, z);
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
