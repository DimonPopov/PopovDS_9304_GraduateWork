#include <QSettings>

#include "controllPanel.h"
#include "ui_controllPanel.h"

#define SETTING_FIRST_GROUP       ("Settings")
#define SENSOR_COUNT              ("SensorCount")
#define SENSOR_POINT_SIZE         ("SensorPointSize")
#define SENSOR_POINT_COLOR        ("SensorPointColor")
#define INTERPOLATION_POINT_SIZE  ("InterpolationPointSize")
#define INTERPOLATION_POINT_COLOR ("InterpolationPointColor")
#define INTERPOLATION_COUNT       ("InterpolationCount")

enum Color {
    Undefine = -1,
    Red,
    Green,
    Blue,
    Count
};

QString colorToStr(const Color& color)
{
    switch (color)
    {
        case Red:   return "red";
        case Green: return "green";
        case Blue:
        default:    return "blue";
    }
}



ControllPanel::ControllPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControllPanel),
    m_settings(new QSettings("GraduateWorkPopov9304", "Scatter", this))
{
    ui->setupUi(this);

    for (int i = 0; i < Color::Count; ++i)
    {
        ui->interpolationPointColorCombo->addItem(colorToStr(static_cast<Color>(i)), i);
        ui->sensorPointColorCombo->addItem(colorToStr(static_cast<Color>(i)), i);
    }

    load();

    ui->startStopButton->setCheckable(true);

    connect(ui->interpolationPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationPointColorChanged);

    connect(ui->sensorPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleSensorPointColorChanged);

    connect(ui->sensorPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigSensorPointSizeChanged);

    connect(ui->interpolationPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationPointSizeChanged);

    connect(ui->sensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigSensorCountChanged);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &ControllPanel::handleEmulationButtonChange);

    connect(ui->interpolationPointsSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationCountChanged);
}

ControllPanel::~ControllPanel()
{
    save();
    delete ui;
}

void ControllPanel::load()
{
    m_settings->beginGroup(SETTING_FIRST_GROUP);
        ui->sensorCountSpin->setValue(m_settings->value(SENSOR_COUNT, 0).toInt());
        ui->sensorPointSizeSpin->setValue(m_settings->value(SENSOR_POINT_SIZE, 0.3f).toDouble());
        ui->sensorPointColorCombo->setCurrentIndex(m_settings->value(SENSOR_POINT_COLOR, 0).toInt());
        ui->interpolationPointSizeSpin->setValue(m_settings->value(INTERPOLATION_POINT_SIZE, 0.3f).toDouble());
        ui->interpolationPointColorCombo->setCurrentIndex(m_settings->value(INTERPOLATION_POINT_COLOR, 0).toInt());
        ui->interpolationPointsSpin->setValue(m_settings->value(INTERPOLATION_COUNT, 0).toInt());
    m_settings->endGroup();
}

void ControllPanel::save()
{
    m_settings->beginGroup(SETTING_FIRST_GROUP);
        m_settings->setValue(SENSOR_COUNT, ui->sensorCountSpin->value());
        m_settings->setValue(SENSOR_POINT_SIZE, ui->sensorPointSizeSpin->value());
        m_settings->setValue(SENSOR_POINT_COLOR, ui->sensorPointColorCombo->currentData());
        m_settings->setValue(INTERPOLATION_POINT_SIZE, ui->interpolationPointSizeSpin->value());
        m_settings->setValue(INTERPOLATION_POINT_COLOR, ui->interpolationPointColorCombo->currentData());
        m_settings->setValue(INTERPOLATION_COUNT, ui->interpolationPointsSpin->value());
    m_settings->endGroup();
}

void ControllPanel::emitAllSignal()
{
    handleSensorPointColorChanged(ui->sensorPointColorCombo->currentIndex());
    handleInterpolationPointColorChanged(ui->interpolationPointColorCombo->currentIndex());
    emit sigSensorCountChanged(ui->sensorCountSpin->value());
    emit sigSensorPointSizeChanged(ui->sensorPointSizeSpin->value());
    emit sigInterpolationPointSizeChanged(ui->interpolationPointSizeSpin->value());
    emit sigInterpolationCountChanged(ui->interpolationPointsSpin->value());
}

void ControllPanel::handleSensorPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->sensorPointColorCombo->currentText());
    emit sigSensorPointColorChanged(newColor);
}

void ControllPanel::handleInterpolationPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->interpolationPointColorCombo->currentText());
    emit sigInterpolationPointColorChanged(newColor);
}

void ControllPanel::handleEmulationButtonChange(const bool& checked)
{
    checked ? ui->startStopButton->setText(tr("Stop")) : ui->startStopButton->setText(tr("Start"));
//    ui->sensorCountSpin->setEnabled(!checked);
    ui->interpolationPointsSpin->setEnabled(!checked);

    emit sigEmulationButtonClicked(checked);
}
