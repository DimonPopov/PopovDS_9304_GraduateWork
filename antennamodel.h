#ifndef ANTENNAMODEL_H
#define ANTENNAMODEL_H

#include <QObject>
#include <QVector3D>


namespace AntennaModelSpace {

class AntennaModel : public QObject
{
    Q_OBJECT
public:
    explicit AntennaModel(const double& lenght,
                          const QVector3D& maxNoice = QVector3D(),
                          QObject* parent = nullptr);
    double getLenght() const;
    void setLenght(double newLenght);
    QVector3D getNewPointPosition(const double& step,
                                  const quint32& pointNumber,
                                  const bool& needNoise = false) const;
private:
    double m_lenght;
    QVector<QVector3D> m_noise;

public slots:
    void handleMaxNoiseChanged(const QVector3D& maxNoise);

signals:
    void sigLenghtChanged(const double& newLenght);
};

}



#endif // ANTENNAMODEL_H
