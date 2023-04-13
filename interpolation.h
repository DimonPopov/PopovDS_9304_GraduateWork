#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <QVector3D>
#include <QScatterDataProxy>



namespace InterpolaionSpace {

// https://www.boost.org/doc/libs/1_81_0/libs/math/doc/html/interpolation.html
enum InterpolationType : quint32 {
//    CardinalQuinticBSpline,          ///< B-сплайновая интерполяция Cardinal Quintic
//    VectorValuedBarycentricRational, ///< Барицентрическая рациональная интерполяция с векторным знаком
//    BezierPolynomials,               ///< Полиномы Безье
//    CardinalTrigonometric,           ///< Кардинальная тригонометрическая интерполяция
//    QuinticHermite,                  ///< Интерполяция Quintic Hermite
//    BilinearUniform,                 ///< Билинейная равномерная интерполяция
    BarycentricRational,             ///< Барицентрическая рациональная интерполяция
    CardinalCubicBSpline,            ///< Кардинальная кубическая интерполяция B-сплайна
    CardinalQuadraticBSpline,        ///< Кардинально-квадратичная B-сплайновая интерполяция
    WhittakerShannon,                ///< Интерполяция Уиттакера-Шеннона
    CatmullRomSplines,               ///< Сплайны Кэтмулла-Рома
    CubicHermite,                    ///< Кубическая интерполяция Эрмита
    ModifiedAkima,                   ///< Модифицированная интерполяция Akima
    PCHIP,                           ///< Интерполяция PCHIP
    Count
};

QString getStrFromType(const InterpolationType& type);


QList<QVector3D> calculateInterpolation(QScatterDataArray* scatterArray,
                                        const double& lenght,
                                        const quint32& size,
                                        const InterpolationType type = InterpolationType::BarycentricRational);
}


#endif // INTERPOLATION_H
