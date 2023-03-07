#ifndef CONTROLLPANEL_H
#define CONTROLLPANEL_H

#include <QWidget>

namespace Ui {
class ControllPanel;
}

class ControllPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControllPanel(QWidget *parent = nullptr);
    bool load();
    bool save();
    ~ControllPanel();

private:
    Ui::ControllPanel *ui;

signals:
    void sigSensorPointColorChanged(const QColor& color);
    void sigSensorCountChanged(const quint32& count);
    void sigSensorPointSizeChanged(const double& sensorPointSize);
    void sigInterpolationPointColorChanged(const QColor& color);
    void sigInterpolationPointSizeChanged(const double& interpolationPointSize);

private slots:
    void handleSensorPointColorChanged(const int& index);
    void handleInterpolationPointColorChanged(const int& index);
};

#endif // CONTROLLPANEL_H
