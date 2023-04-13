#include <boost/math/interpolators/barycentric_rational.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_quadratic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_quintic_b_spline.hpp>
#include <boost/math/interpolators/whittaker_shannon.hpp>
#include <boost/math/interpolators/catmull_rom.hpp>
#include <boost/math/interpolators/bezier_polynomial.hpp>
#include <boost/math/interpolators/cardinal_trigonometric.hpp>
#include <boost/math/interpolators/cubic_hermite.hpp>
#include <boost/math/interpolators/makima.hpp>
#include <boost/math/interpolators/pchip.hpp>

#include "interpolation.h"


template <class T, class X>
QList<QVector3D> getResult(T& _i1, X& _i2, const float& _step, const quint32& _size)
{
    QList<QVector3D> result;

    for (unsigned i = 0; i < _size; ++i)
    {
        double Y = _i1(i * _step);
        double Z = _i2(i * _step);
        result << QVector3D(i * _step, Y, Z);
    }

    return result;
}



template <class T>
QList<QVector3D> getResult(T& _i1, const float& _step, const quint32& _size)
{
    QList<QVector3D> result;

    for (unsigned i = 0; i < _size; ++i)
    {
        auto position = _i1(i * _step);
        result << QVector3D(position[0], position[1], position[2]);
    }

    return result;
}



std::vector<std::array<double, 3>> packInVectorArray(std::vector<double>& x,
                                                     std::vector<double>& y,
                                                     std::vector<double>& z)
{
    std::vector<std::array<double, 3>> points;

    for (unsigned i = 0; i < x.size(); ++i)
        points.push_back({x[i], y[i], z[i]});

    return points;
}


//static QList<QVector3D> quinticHermite(std::vector<double>& x,
//                                       std::vector<double>& y,
//                                       std::vector<double>& z,
//                                       const double& lenght,
//                                       const quint32& size)
//{

//}


//static QList<QVector3D> bezierPolynomials(std::vector<double>& x,
//                                          std::vector<double>& y,
//                                          std::vector<double>& z,
//                                          const quint32& size)
//{
//    using namespace boost::math::interpolators;

//    std::vector<std::array<double, 3>> points;

//    for (unsigned i = 0; i < x.size(); ++i)
//        points.push_back({x[i], y[i], z[i]});

//    auto interpolator = bezier_polynomial(std::move(points));

//    QList<QVector3D> result;

//    float step = 1.0f / size;

//    for (unsigned i = 0; i < size; ++i)
//    {
//        auto position = interpolator(i * step);
//        qDebug() << position[0] << position[1] << position[2] <<position.size();
//        result << QVector3D(position[0], position[1], position[2]);
//    }
//    qDebug() << '\n';

//    return result;
//}



//static QList<QVector3D> cardinalTrigonometric(std::vector<double>& x,
//                                              std::vector<double>& y,
//                                              std::vector<double>& z,
//                                              const double& lenght,
//                                              const quint32& size)
//{
//    using boost::math::interpolators::cardinal_trigonometric;

//    auto interpolatorX = cardinal_trigonometric<std::vector<double>>(x, 0, 0.1);
//    auto interpolatorY = cardinal_trigonometric<std::vector<double>>(y, 0, 0.1);
//    auto interpolatorZ = cardinal_trigonometric<std::vector<double>>(z, 0, 0.1);

//    QList<QVector3D> result;

//    float step = lenght / size;

//    for (unsigned i = 0; i < size; ++i)
//        result << QVector3D(interpolatorX(i * step), interpolatorY(i * step), interpolatorZ(i * step));

//    return result;
//}



//static QList<QVector3D> cardinalQuinticBSpline(std::vector<double>& x,
//                                               std::vector<double>& y,
//                                               std::vector<double>& z,
//                                               const double& lenght,
//                                               const quint32& size)
//{
//    using namespace boost::math::interpolators;

//    auto interpolatorX = cardinal_quintic_b_spline<double>(y, 0, x[1]);
//    auto interpolatorZ = cardinal_quintic_b_spline<double>(z, 0, x[1]);

//    QList<QVector3D> result;

//    float step = lenght / size;

//    for (unsigned i = 0; i < size; ++i)
//    {
//        double Y = interpolatorX(i * step);
//        double Z = interpolatorZ(i * step);
//        result << QVector3D(i * step, Y, Z);
//    }

//    return result;
//}



QList<QVector3D> InterpolaionSpace::calculateInterpolation(QScatterDataArray* scatterArray,
                                                           const double& lenght,
                                                           const quint32& size,
                                                           const InterpolationType type)
{
    using namespace boost::math::interpolators;
    using namespace boost::math;

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
//        case InterpolationType::CardinalQuinticBSpline:             return cardinalQuinticBSpline(x, y, z, lenght, size);      // Не работает.
//        case InterpolationType::CardinalTrigonometric:              return cardinalTrigonometric(x, y, z, lenght, size);       // Получается замкнутая хрень.
//        case InterpolationType::QuinticHermite:                     return quinticHermite(x, y, z, lenght, size);              // Чет с производными
//        case InterpolationType::BilinearUniform:
//        case InterpolationType::VectorValuedBarycentricRational:
        //
//        case InterpolationType::BezierPolynomials:
//    {
//            std::vector<std::array<double, 3>> points;
//            for (unsigned i = 0; i < x.size(); ++i)
//                points.push_back({x[i], y[i], z[i]});

//            auto interpolator = bezier_polynomial(std::move(points));
//            float step = 1.0f / size;
//            QList<QVector3D> result;

//            for (unsigned i = 0; i < size; ++i)
//            {
//                auto position = interpolator(i * step);
//                result << QVector3D(position[0], position[1], position[2]);
//            }
//            return result;

//    }
        case InterpolationType::CardinalCubicBSpline:
        {
            auto interpolatorY = cardinal_cubic_b_spline<double>(y.begin(), y.end(), 0, x[1]);
            auto interpolatorZ = cardinal_cubic_b_spline<double>(z.begin(), z.end(), 0, x[1]);
            return getResult(interpolatorY, interpolatorZ, lenght / size, size);
        }
        case InterpolationType::CardinalQuadraticBSpline:
        {
            auto interpolatorY = cardinal_quadratic_b_spline<double>(y, 0, x[1]);
            auto interpolatorZ = cardinal_quadratic_b_spline<double>(z, 0, x[1]);
            return getResult(interpolatorY, interpolatorZ, lenght / size, size);
        }
        case InterpolationType::WhittakerShannon:
        {
            auto interpolatorY = whittaker_shannon(std::move(y), 0, x[1]);
            auto interpolatorZ = whittaker_shannon(std::move(z), 0, x[1]);
            return getResult(interpolatorY, interpolatorZ, lenght / size, size);
        }
        case InterpolationType::CatmullRomSplines:
        {
            auto points = packInVectorArray(x, y, z);
            auto interpolator = catmull_rom<std::array<double, 3>>(std::move(points));
            return getResult(interpolator, interpolator.max_parameter() / size, size);
        }
        case InterpolationType::CubicHermite:
        {
            std::vector<double> dxdy1(x.size());
            std::vector<double> dxdy2(x.size());
            std::vector<double> duplicate = x;
            const float step = x.back() / size;
            auto interpolatorY = cubic_hermite(std::move(x),         std::move(y), std::move(dxdy1));
            auto interpolatorZ = cubic_hermite(std::move(duplicate), std::move(z), std::move(dxdy2));
            return getResult(interpolatorY, interpolatorZ, step, size);
        }
        case InterpolationType::ModifiedAkima:
        {
            std::vector<double> duplicate = x;
            const float step = x.back() / size;
            auto interpolatorY = makima(std::move(x),         std::move(y));
            auto interpolatorZ = makima(std::move(duplicate), std::move(z));
            return getResult(interpolatorY, interpolatorZ, step, size);
        }
        case InterpolationType::PCHIP:
        {
            std::vector<double> duplicate = x;
            const float step = x.back() / size;
            auto interpolatorY = pchip(std::move(x),         std::move(y));
            auto interpolatorZ = pchip(std::move(duplicate), std::move(z));
            return getResult(interpolatorY, interpolatorZ, step, size);
        }
        case InterpolationType::BarycentricRational:
        {
            std::vector<double> duplicate = x;
            auto interpolatorY = barycentric_rational<double>(std::move(x),         std::move(y));
            auto interpolatorZ = barycentric_rational<double>(std::move(duplicate), std::move(z));
            return getResult(interpolatorY, interpolatorZ, lenght / size, size);
        }
        default:    return QList<QVector3D>();
    }
}



QString InterpolaionSpace::getStrFromType(const InterpolationType &type)
{
    switch (type)
    {
//        case InterpolationType::CardinalQuinticBSpline:             return QString("Cardinal Quintic B-Spline");
//        case InterpolationType::BezierPolynomials:                  return QString("Bezier Polynomials");
//        case InterpolationType::CardinalTrigonometric:              return QString("Cardinal Trigonometric");
//        case InterpolationType::QuinticHermite:                     return QString("Quintic Hermite");
//        case InterpolationType::BilinearUniform:                    return QString("Bilinear Uniform");
//        case InterpolationType::VectorValuedBarycentricRational:    return QString("Vector Valued Barycentric Rational");
        case InterpolationType::CardinalCubicBSpline:               return QString("Cardinal Cubic B-Spline");
        case InterpolationType::CardinalQuadraticBSpline:           return QString("Cardinal Quadratic B-Spline");
        case InterpolationType::WhittakerShannon:                   return QString("Whittaker Shannon");
        case InterpolationType::CatmullRomSplines:                  return QString("Catmull Rom Splines");
        case InterpolationType::CubicHermite:                       return QString("Cubic Hermite");
        case InterpolationType::ModifiedAkima:                      return QString("Modified Akima");
        case InterpolationType::PCHIP:                              return QString("PCHIP");
        case InterpolationType::BarycentricRational:                return QString("Barycentric Rational");
        default:                                                    return QString("");
    }
}
