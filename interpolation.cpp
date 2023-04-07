#include <boost/math/interpolators/barycentric_rational.hpp>

#include "interpolation.h"



static QList<QVector3D> barycentricRational(QSharedPointer<QScatterDataArray> scatterArray,
                                            const double& lenght,
                                            const double& size);


QList<QVector3D> InterpolaionSpace::calculateInterpolation(QSharedPointer<QScatterDataArray> scatterArray,
                                                           const double& lenght,
                                                           const double& size,
                                                           const InterpolationType type)
{
    switch (type)
    {
        case InterpolationType::CardinalCubicBSpline:
        case InterpolationType::CardinalQuadraticBSpline:
        case InterpolationType::CardinalQuinticBSpline:
        case InterpolationType::WhittakerShannon:
        case InterpolationType::BarycentricRational:
        case InterpolationType::CatmullRomSplines:
        case InterpolationType::BezierPolynomials:
        case InterpolationType::CardinalTrigonometric:
        case InterpolationType::CubicHermite:
        case InterpolationType::ModifiedAkima:
        case InterpolationType::PCHIP:
        case InterpolationType::QuinticHermite:
        case InterpolationType::BilinearUniform:
        case InterpolationType::VectorValuedBarycentricRational:
        default: return barycentricRational(scatterArray, lenght, size);
    }
}

static QList<QVector3D> barycentricRational(QSharedPointer<QScatterDataArray> scatterArray,
                                            const double& lenght,
                                            const double& size)
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


    using namespace boost::math::interpolators;

    std::vector<double> duplicate = x;

    auto interpolatorX = barycentric_rational<double>(std::move(x), std::move(y));
    auto interpolatorZ = barycentric_rational<double>(std::move(duplicate), std::move(z));

    double step = lenght / static_cast<double>(size);

    QList<QVector3D> result;

    for (unsigned i = 0; i < size; ++i)
    {
        double Y = interpolatorX(i * step);
        double Z = interpolatorZ(i * step);
        result << QVector3D(i * step, Y, Z);
    }

    return result;
}
