#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <QVector3D>
#include <QScatterDataProxy>



namespace InterpolaionSpace {

enum InterpolationType {
    CardinalCubicBSpline,            ///< Кардинальная кубическая интерполяция B-сплайна
    CardinalQuadraticBSpline,        ///< Кардинально-квадратичная B-сплайновая интерполяция
    CardinalQuinticBSpline,          ///< B-сплайновая интерполяция Cardinal Quintic
    WhittakerShannon,                ///< Интерполяция Уиттакера-Шеннона
    BarycentricRational,             ///< Барицентрическая рациональная интерполяция
    VectorValuedBarycentricRational, ///< Барицентрическая рациональная интерполяция с векторным знаком
    CatmullRomSplines,               ///< Сплайны Кэтмулла-Рома
    BezierPolynomials,               ///< Полиномы Безье
    CardinalTrigonometric,           ///< Кардинальная тригонометрическая интерполяция
    CubicHermite,                    ///< Кубическая интерполяция Эрмита
    ModifiedAkima,                   ///< Модифицированная интерполяция Akima
    PCHIP,                           ///< Интерполяция PCHIP
    QuinticHermite,                  ///< Интерполяция Quintic Hermite
    BilinearUniform                  ///< Билинейная равномерная интерполяция
};


QList<QVector3D> calculateInterpolation(QSharedPointer<QScatterDataArray> scatterArray,
                                        const double& lenght,
                                        const double& size,
                                        const InterpolationType type = InterpolationType::BarycentricRational);
}


#endif // INTERPOLATION_H
