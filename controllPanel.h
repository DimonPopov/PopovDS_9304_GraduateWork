#ifndef CONTROLLPANEL_H
#define CONTROLLPANEL_H

#include <QWidget>

#include "interpolation.h"



class QSettings;

namespace Ui {
class ControllPanel;
}

class ControllPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControllPanel(QWidget *parent = nullptr);
    ~ControllPanel();
    void load();
    void save();
    double getAntennaLenght() const;
    double getSensorSize() const;
    double getInterpolationSize() const;
    double getTrueModelSize() const;
    quint32 getSensorCount() const;
    quint32 getInterpolationCount() const;
    quint32 getTrueModelCount() const;
    InterpolaionSpace::InterpolationType getInterpolationType() const;
    QColor getSensorColor() const;
    QColor getInterpolationColor() const;
    QColor getTrueModelColor() const;
    int getAntennaVisibility() const;
    int getSensorVisibility() const;
    int getInterpolationVisibility() const;
    bool getSensorEnd() const;

private:
    Ui::ControllPanel *ui;
    QSettings* m_settings;

signals:
    void sigSensorCountChanged(const quint32& count);
    void sigAntennaLenghtChanged(const double& lenght);
    void sigSensorPointColorChanged(const QColor& color);
    void sigSensorPointSizeChanged(const double& sensorPointSize);
    void sigInterpolationPointColorChanged(const QColor& color);
    void sigInterpolationPointSizeChanged(const double& interpolationPointSize);
    void sigEmulationButtonClicked(const bool& state);
    void sigInterpolationCountChanged(const quint32& count);
    void sigMaxDeviationChanged(const double& value);
    void sigAntennaVisibilityChanged(const bool& state);
    void sigSensorVisibilityChanged(const bool& state);
    void sigInterpolationVisibilityChanged(const bool& state);
    void sigTrueModelPointColorChanged(const QColor& color);
    void sigTrueModelPointSizeChanged(const double& trueModelPointSize);
    void sigTrueModelCountChanged(const quint32& count);
    void sigInterpolationTypeChanged(const InterpolaionSpace::InterpolationType& type);
    void sigSensorEndChanged(const bool& state);

private slots:
    void handleSensorPointColorChanged(const int& index);
    void handleInterpolationPointColorChanged(const int& index);
    void handleTrueModelColorChanged(const int& index);
    void handleEmulationButtonChange(const bool& checked);
    void handleVisibilityCheckBoxsChanged(const bool& checkState);
    void handleInterpolationTypeChanged(const int& index);
};

#endif // CONTROLLPANEL_H
