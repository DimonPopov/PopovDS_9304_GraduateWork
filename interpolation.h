#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <QVector3D>
#include <QScatterDataProxy>



namespace InterpolaionSpace {

// https://www.boost.org/doc/libs/1_81_0/libs/math/doc/html/interpolation.html
enum InterpolationType : quint32 {
    BarycentricRational,             ///< Барицентрическая рациональная интерполяция
    CardinalCubicBSpline,            ///< Кардинальная кубическая интерполяция B-сплайна
    CardinalQuadraticBSpline,        ///< Кардинально-квадратичная B-сплайновая интерполяция
//    CardinalQuinticBSpline,          ///< B-сплайновая интерполяция Cardinal Quintic
    WhittakerShannon,                ///< Интерполяция Уиттакера-Шеннона
//    VectorValuedBarycentricRational, ///< Барицентрическая рациональная интерполяция с векторным знаком
    CatmullRomSplines,               ///< Сплайны Кэтмулла-Рома
//    BezierPolynomials,               ///< Полиномы Безье
//    CardinalTrigonometric,           ///< Кардинальная тригонометрическая интерполяция
    CubicHermite,                    ///< Кубическая интерполяция Эрмита
    ModifiedAkima,                   ///< Модифицированная интерполяция Akima
    PCHIP,                           ///< Интерполяция PCHIP
//    QuinticHermite,                  ///< Интерполяция Quintic Hermite
//    BilinearUniform,                 ///< Билинейная равномерная интерполяция
    Count
};

QString getStrFromType(const InterpolationType& type);


QList<QVector3D> calculateInterpolation(QSharedPointer<QScatterDataArray> scatterArray,
                                        const double& lenght,
                                        const quint32& size,
                                        const InterpolationType type = InterpolationType::BarycentricRational);
}


#endif // INTERPOLATION_H
