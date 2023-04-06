#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <QObject>
#include <QVector3D>
#include <QList>
#include <vector>



namespace InterpolatorSpace {

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

class Interpolator : public QObject
{
    Q_OBJECT
public:
    Interpolator(const quint32& amountPoints, const double& lenght, QObject* parent = nullptr);
    quint32 getInterpolationCount() const;
    void setLenght(const double& newLenght);
    QList<QVector3D> calculateInterpolation(std::vector<double>& x,
                                            std::vector<double>& y,
                                            std::vector<double>& z,
                                            const InterpolationType type = InterpolationType::BarycentricRational) const;
private:
    quint32 m_interpolationCount;
    double  m_lenght;
    QList<QVector3D> barycentricRational(std::vector<double> &x,
                                         std::vector<double> &y,
                                         std::vector<double> &z,
                                         QList<QVector3D>& result) const;

public slots:
    void handleInterpolationCountChanged(const quint32& newCount);
    void handleAntennaLenghtChanged(const double& newLenght);

signals:
    void sigInterpolatorChanged();
};

}

#endif // INTERPOLATOR_H
