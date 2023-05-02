#ifndef ERRORCOMPLEXITYHANDLER_H
#define ERRORCOMPLEXITYHANDLER_H

#include <QObject>
#include <QElapsedTimer>
#include <QVector3D>
#include <QScatterDataProxy>



class QElapsedTimer;

class ErrorComplexityHandler : public QObject
{
    Q_OBJECT
public:
    ErrorComplexityHandler(QObject* parent = nullptr);
    void startTimer();
    void stopTimer();
    int getTime(const quint32& divider = 1'000) const;
    QVector3D getMaxDeviation() const;
private:
    qint32 m_time;
    QVector3D m_maxDeviation;
    QScopedPointer<QElapsedTimer> m_timer;
};

#endif // ERRORCOMPLEXITYHANDLER_H
