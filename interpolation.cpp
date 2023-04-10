#include <boost/math/interpolators/barycentric_rational.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_quadratic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_quintic_b_spline.hpp>
#include <boost/math/interpolators/whittaker_shannon.hpp>

#include "interpolation.h"



static QList<QVector3D> barycentricRational(std::vector<double>& x,
                                            std::vector<double>& y,
                                            std::vector<double>& z,
                                            const double& lenght,
                                            const quint32& size)
{
    using namespace boost::math::interpolators;

    std::vector<double> duplicate = x;

    auto interpolatorX = barycentric_rational<double>(std::move(x), std::move(y));
    auto interpolatorZ = barycentric_rational<double>(std::move(duplicate), std::move(z));

    double step = lenght / size;

    QList<QVector3D> result;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}



static QList<QVector3D> cardinalCubicBSpline(std::vector<double>& y,
                                             std::vector<double>& z,
                                             const double& lenght,
                                             const quint32& size)
{
    using namespace boost::math::interpolators;

    auto interpolatorX = cardinal_cubic_b_spline<double>(y.begin(), y.end(), 0, lenght / y.size());
    auto interpolatorZ = cardinal_cubic_b_spline<double>(z.begin(), z.end(), 0, lenght / z.size());

    QList<QVector3D> result;

    double step = lenght / size;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}



static QList<QVector3D> cardinalQuadraticBSpline(std::vector<double>& y,
                                                 std::vector<double>& z,
                                                 const double& lenght,
                                                 const quint32& size)
{
    using namespace boost::math::interpolators;

    auto interpolatorX = cardinal_quadratic_b_spline<double>(y, 0, lenght / y.size());
    auto interpolatorZ = cardinal_quadratic_b_spline<double>(z, 0, lenght / z.size());

    QList<QVector3D> result;

    double step = lenght / size;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}



static QList<QVector3D> cardinalQuinticBSpline(std::vector<double>& y,
                                               std::vector<double>& z,
                                               const double& lenght,
                                               const quint32& size)
{
    using namespace boost::math::interpolators;

    auto interpolatorX = cardinal_quintic_b_spline<double>(y, 0, lenght / y.size());
    auto interpolatorZ = cardinal_quintic_b_spline<double>(z, 0, lenght / z.size());

    QList<QVector3D> result;

    double step = lenght / size;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}



static QList<QVector3D> whittakerShannon(std::vector<double>& y,
                                         std::vector<double>& z,
                                         const double& lenght,
                                         const quint32& size)
{
    using namespace boost::math::interpolators;

    auto interpolatorX = whittaker_shannon(std::move(y), 0, lenght / y.size());
    auto interpolatorZ = whittaker_shannon(std::move(z), 0, lenght / z.size());

    QList<QVector3D> result;

    double step = lenght / size;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}


static QList<QVector3D> catmullRomSplines(std::vector<double>& x,
                                          std::vector<double>& y,
                                          std::vector<double>& z,
                                          const double& lenght,
                                          const quint32& size)
{

}


QList<QVector3D> InterpolaionSpace::calculateInterpolation(QSharedPointer<QScatterDataArray> scatterArray,
                                                           const double& lenght,
                                                           const quint32& size,
                                                           const InterpolationType type)
{
    const quint32 vectorSize = scatterArray->size();

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;

    x.reserve(vectorSize);
    y.reserve(vectorSize);
    z.reserve(vectorSize);

    for (const auto& p : *scatterArray)
    {
        x.emplace_back(p.x());
        y.emplace_back(p.y());
        z.emplace_back(p.z());
    }

    switch (type)
    {
        case InterpolationType::CardinalCubicBSpline:               return cardinalCubicBSpline(y, z, lenght, size);
        case InterpolationType::CardinalQuadraticBSpline:           return cardinalQuadraticBSpline(y, z, lenght, size);
        case InterpolationType::CardinalQuinticBSpline:             return cardinalQuinticBSpline(y, z, lenght, size); // Чет не робит
        case InterpolationType::WhittakerShannon:                   return whittakerShannon(y, z, lenght, size);
        case InterpolationType::CatmullRomSplines:                  return catmullRomSplines(x, y, z, lenght, size);
        case InterpolationType::BezierPolynomials:
        case InterpolationType::CardinalTrigonometric:
        case InterpolationType::CubicHermite:
        case InterpolationType::ModifiedAkima:
        case InterpolationType::PCHIP:
        case InterpolationType::QuinticHermite:
        case InterpolationType::BilinearUniform:
        case InterpolationType::VectorValuedBarycentricRational:
        case InterpolationType::BarycentricRational:
        default:                                                    return barycentricRational(x, y, z, lenght, size);
    }
}

QString InterpolaionSpace::getStrFromType(const InterpolationType &type)
{
    switch (type)
    {
        case InterpolationType::CardinalCubicBSpline:               return QString("Cardinal Cubic B-Spline");
        case InterpolationType::CardinalQuadraticBSpline:           return QString("Cardinal Quadratic B-Spline");
        case InterpolationType::CardinalQuinticBSpline:             return QString("Cardinal Quintic B-Spline");
        case InterpolationType::WhittakerShannon:                   return QString("Whittaker Shannon");
        case InterpolationType::CatmullRomSplines:                  return QString("Catmull Rom Splines");
        case InterpolationType::BezierPolynomials:                  return QString("Bezier Polynomials");
        case InterpolationType::CardinalTrigonometric:              return QString("Cardinal Trigonometric");
        case InterpolationType::CubicHermite:                       return QString("Cubic Hermite");
        case InterpolationType::ModifiedAkima:                      return QString("Modified Akima");
        case InterpolationType::PCHIP:                              return QString("PCHIP");
        case InterpolationType::QuinticHermite:                     return QString("Quintic Hermite");
        case InterpolationType::BilinearUniform:                    return QString("Bilinear Uniform");
        case InterpolationType::VectorValuedBarycentricRational:    return QString("Vector Valued Barycentric Rational");
        case InterpolationType::BarycentricRational:                return QString("Barycentric Rational");
        default:                                                    return QString("");
    }
}
