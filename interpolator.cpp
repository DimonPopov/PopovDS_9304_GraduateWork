#include <boost/math/interpolators/barycentric_rational.hpp>

#include "interpolator.h"



InterpolatorSpace::Interpolator::Interpolator(const quint32 &amountPoints, const double& lenght, QObject *parent)
    : QObject(parent),
      m_interpolationCount(amountPoints),
      m_lenght(lenght)
{

}

quint32 InterpolatorSpace::Interpolator::getInterpolationCount() const
{
    return m_interpolationCount;
}

QList<QVector3D> InterpolatorSpace::Interpolator::calculateInterpolation(std::vector<double> &x,
                                                                  std::vector<double> &y,
                                                                  std::vector<double> &z,
                                                                  const InterpolationType type) const
{
    QList<QVector3D> result;
    switch (type)
    {
        case InterpolationType::CardinalCubicBSpline:
        case InterpolationType::CardinalQuadraticBSpline:
        case InterpolationType::CardinalQuinticBSpline:
        case InterpolationType::WhittakerShannon:
        case InterpolationType::BarycentricRational:             return barycentricRational(x, y, z, result);
        case InterpolationType::VectorValuedBarycentricRational:
        case InterpolationType::CatmullRomSplines:
        case InterpolationType::BezierPolynomials:
        case InterpolationType::CardinalTrigonometric:
        case InterpolationType::CubicHermite:
        case InterpolationType::ModifiedAkima:
        case InterpolationType::PCHIP:
        case InterpolationType::QuinticHermite:
        case InterpolationType::BilinearUniform:
        default: return result;
    }
}

QList<QVector3D> InterpolatorSpace::Interpolator::barycentricRational(std::vector<double> &x,
                                                               std::vector<double> &y,
                                                               std::vector<double> &z,
                                                               QList<QVector3D> &result) const
{
    using namespace boost::math::interpolators;

    std::vector<double> duplicate = x;

    auto interpolatorX = barycentric_rational<double>(std::move(x), std::move(y));
    auto interpolatorZ = barycentric_rational<double>(std::move(duplicate), std::move(z));

    double step = m_lenght / static_cast<double>(m_interpolationCount);

    for (unsigned i = 0; i < m_interpolationCount; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}

void InterpolatorSpace::Interpolator::handleInterpolationCountChanged(const quint32 &newCount)
{
    m_interpolationCount = newCount;
    emit sigInterpolatorChanged();
}

void InterpolatorSpace::Interpolator::handleAntennaLenghtChanged(const double &newLenght)
{
    m_lenght = newLenght;
}
