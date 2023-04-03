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
#define MAX_DEVIATION             ("MaxDeviation")
#define ANTENNA_LENGTH            ("AntennaLength")

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

//    load();

    ui->startStopButton->setCheckable(true);
    ui->sensorPointColorCombo->setCurrentIndex(-1);
    ui->interpolationPointColorCombo->setCurrentIndex(-1);

    connect(ui->sensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::handleSensorDataChanged);

    connect(ui->antennaLengthSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::handleSensorDataChanged);

    connect(ui->interpolationPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationPointColorChanged);

    connect(ui->sensorPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleSensorPointColorChanged);

    connect(ui->sensorPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigSensorPointSizeChanged);

    connect(ui->interpolationPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationPointSizeChanged);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &ControllPanel::handleEmulationButtonChange);

    connect(ui->interpolationPointsSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationCountChanged);

    connect(ui->maximumDeviationSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigMaxDeviationChanged);

    connect(ui->antennaVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->sensorsVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->interpolationVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);
}

ControllPanel::~ControllPanel()
{
    save();
    delete ui;
}

void ControllPanel::load()
{
    m_settings->beginGroup(SETTING_FIRST_GROUP);
        ui->sensorCountSpin->setValue(m_settings->value(SENSOR_COUNT, 4).toInt());
        ui->antennaLengthSpin->setValue(m_settings->value(ANTENNA_LENGTH, 10.0f).toDouble());
        ui->sensorPointSizeSpin->setValue(m_settings->value(SENSOR_POINT_SIZE, 0.3f).toDouble());
        ui->sensorPointColorCombo->setCurrentIndex(m_settings->value(SENSOR_POINT_COLOR, 0).toInt());
        ui->interpolationPointSizeSpin->setValue(m_settings->value(INTERPOLATION_POINT_SIZE, 0.3f).toDouble());
        ui->interpolationPointColorCombo->setCurrentIndex(m_settings->value(INTERPOLATION_POINT_COLOR, 0).toInt());
        ui->interpolationPointsSpin->setValue(m_settings->value(INTERPOLATION_COUNT, 0).toInt());
        ui->maximumDeviationSpin->setValue(m_settings->value(MAX_DEVIATION, 0.1f).toDouble());
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
        m_settings->setValue(MAX_DEVIATION, ui->maximumDeviationSpin->value());
        m_settings->setValue(ANTENNA_LENGTH, ui->antennaLengthSpin->value());
    m_settings->endGroup();
}

void ControllPanel::handleSensorDataChanged()
{
    const int sensCount = ui->sensorCountSpin->value();
    const double antLenght = ui->antennaLengthSpin->value();

    if (sensCount < 4 || antLenght == 0)
        return;

    emit sigSensorDataChanged({ui->sensorCountSpin->value(), ui->antennaLengthSpin->value()});
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
    emit sigEmulationButtonClicked(checked);
}

void ControllPanel::handleVisibilityCheckBoxsChanged(const bool &checkState)
{
    Q_UNUSED(checkState);

//    sender();
}
