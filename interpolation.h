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

/**
 * @brief Функция получения названия интерполяции.
 * @param type - тип интерполяции.
 * @return название интерполяции в формате QString.
 */
QString getStrFromType(const InterpolationType& type);



/**
 * @brief Функция интерполяции.
 * @param scatterArray - координаты датчиков позиции.
 * @param lenght - длинна модели антенны.
 * @param size - количество акустических датчиков (точек интерполяции).
 * @param type - тип интерполяции.
 * @return координаты акустических датчиков в формате QScatterDataArray.
 */
QScatterDataArray calculateInterpolation(QScatterDataArray* scatterArray,
                                         const double& lenght,
                                         const quint32& size,
                                         const InterpolationType type = InterpolationType::BarycentricRational);
}


#endif // INTERPOLATION_H
