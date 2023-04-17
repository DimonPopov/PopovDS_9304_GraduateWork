#ifndef ANTENNAMODEL_H
#define ANTENNAMODEL_H

#include <QObject>
#include <QVector3D>
#include <QTimer>



namespace AntennaModelSpace {

enum class NoiseType
{
    Nope = -1,
    Model,
    Position
};

class AntennaModel : public QObject
{
    Q_OBJECT
public:
    explicit AntennaModel(const double& lenght,
                          const double& offset,
                          const double& step,
                          const double& interval,
                          const QVector3D& maxNoice = QVector3D(),
                          QObject* parent = nullptr);
    double getLenght() const;
    double getOffest() const;
    double getStep() const;
    double getInterval() const;
    void setLenght(const double& newLenght);
    void setOffest(const double& newOffset);
    void setStep(const double& newStep);
    void setInterval(const double& newInterval);
    QVector3D getNewPointPosition(const double& step,
                                  const quint32& pointNumber,
                                  const NoiseType& type = NoiseType::Nope) const;


private:
    double m_lenght;
    double m_offset;
    double m_step;
    double m_interval;
    QScopedPointer<QTimer> m_timer;
    QVector<QVector3D> m_noise;

public slots:
    void handleMaxNoiseChanged(const QVector3D& maxNoise);
    void handleStartEmulate(const bool& state);

signals:
    void sigLenghtChanged(const double& newLenght);
};

}



#endif // ANTENNAMODEL_H
