#include "antennamodel.h"



AntennaModelSpace::AntennaModel::AntennaModel(const double &lenght, QObject *parent)
    : QObject(parent),
      m_lenght(lenght)
{

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

QVector3D AntennaModelSpace::AntennaModel::getNewPointPosition(const double &step, const quint32 &pointNumber) const
{
    const double x = step * pointNumber;
    const double y = 3 + sin(2 * x + 0.5f);
    const double z = 2 + x * 0.3f;

    return QVector3D(x, y, z);
}
