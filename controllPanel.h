#ifndef CONTROLLPANEL_H
#define CONTROLLPANEL_H

#include <QWidget>
#include <QPair>

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

private:
    Ui::ControllPanel *ui;
    QSettings* m_settings;

signals:
    void sigSensorDataChanged(const QPair<quint32, double>& data);
    void sigSensorPointColorChanged(const QColor& color);
    void sigSensorPointSizeChanged(const double& sensorPointSize);
    void sigInterpolationPointColorChanged(const QColor& color);
    void sigInterpolationPointSizeChanged(const double& interpolationPointSize);
    void sigEmulationButtonClicked(const bool& state);
    void sigInterpolationCountChanged(const quint32& count);
    void sigMaxDeviationChanged(const double& value);

private slots:
    void handleSensorDataChanged();
    void handleSensorPointColorChanged(const int& index);
    void handleInterpolationPointColorChanged(const int& index);
    void handleEmulationButtonChange(const bool& checked);
};

#endif // CONTROLLPANEL_H
