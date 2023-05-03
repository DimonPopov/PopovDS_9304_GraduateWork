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
    quint32 getModelCount() const;
    double getModelLenght() const;
    double getModelSize() const;
    QColor getModelColor() const;
    double getModelInterval() const;
    double getModelStep() const;
    int getModelVisibility() const;
    quint32 getPositionSensorCount() const;
    double getPositionSensorSize() const;
    QColor getPositionSensorColor() const;
    bool getPositionSensorEnd() const;
    bool getPositionSensorNoise() const;
    int getPositionSensorVisibility() const;
    quint32 getAcousticSensorCount() const;
    double getAcousticSensorSize() const;
    QColor getAcousticSensorColor() const;
    int getAcousticSensorVisibility() const;
    QVector3D getMaxNoise() const;
    QPair<double, double> getModel() const;
    InterpolaionSpace::InterpolationType getInterpolationType() const;

private:
    Ui::ControllPanel *ui;
    QSettings* m_settings;

signals:
    void sigModelCountChanged(const quint32& count);
    void sigModelLenghtChanged(const double& lenght);
    void sigModelVisibilityChanged(const bool& state);
    void sigModelColorChanged(const QColor& color);
    void sigModelSizeChanged(const double& trueModelPointSize);
    void sigPositionSensorCountChanged(const quint32& count);
    void sigPositionSensorColorChanged(const QColor& color);
    void sigPositionSensorSizeChanged(const double& sensorPointSize);
    void sigPositionSensorVisibilityChanged(const bool& state);
    void sigPositionSensorEndChanged(const bool& state);
    void sigAcousticSensorCountChanged(const quint32& count);
    void sigAcousticSensorColorChanged(const QColor& color);
    void sigAcousticSensorSizeChanged(const double& interpolationPointSize);
    void sigInterpolationTypeChanged(const InterpolaionSpace::InterpolationType& type);
    void sigAcousticSensorVisibilityChanged(const bool& state);
    void sigEmulationButtonClicked(const bool& state);
    void sigMaxNoiseChanged(const QVector3D& noise);
    void sigIntervalModelChanged(const double& interval);
    void sigStepModelChanged(const double& step);
    void sigNoiseChanged(const bool& noise);
    void sigModelChanged(const QPair<double, double> coef);
    void sigDisplayOptionChanged(const bool& option);

public slots:
    void handleUpdateInterpolationTime(const quint32& time);

private slots:
    void handlePositionSensorColorChanged(const int& index);
    void handleAcousticSensorColorChanged(const int& index);
    void handleModelColorChanged(const int& index);
    void handleVisibilityCheckBoxsChanged(const bool& checkState);
    void handleInterpolationTypeChanged(const int& index);
    void handleEmulationButtonChange(const bool& checked);
    void handleNoiseChanged(const double& value);
    void handleMaxValueNoiseChange();
    //void handleOpenModelSettingsDialog();
    void handleOpenFileDialog();
    void handleModelChanged(const int& index);
    void handleLanguageChanged(const bool& checked);
};

#endif // CONTROLLPANEL_H
