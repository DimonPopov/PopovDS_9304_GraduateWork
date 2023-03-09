#ifndef CONTROLLPANEL_H
#define CONTROLLPANEL_H

#include <QWidget>

class QSettings;

namespace Ui {
class ControllPanel;
}

class ControllPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControllPanel(QWidget *parent = nullptr);
    void load();
    void save();
    void emitAllSignal();
    ~ControllPanel();

private:
    Ui::ControllPanel *ui;
    QSettings* m_settings;

signals:
    void sigSensorPointColorChanged(const QColor& color);
    void sigSensorCountChanged(const quint32& count);
    void sigSensorPointSizeChanged(const double& sensorPointSize);
    void sigInterpolationPointColorChanged(const QColor& color);
    void sigInterpolationPointSizeChanged(const double& interpolationPointSize);
    void sigEmulationButtonClicked(const bool& state);

private slots:
    void handleSensorPointColorChanged(const int& index);
    void handleInterpolationPointColorChanged(const int& index);
    void handleEmulationButtonChange(const bool& checked);
};

#endif // CONTROLLPANEL_H
