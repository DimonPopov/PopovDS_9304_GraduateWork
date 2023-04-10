#include <QSettings>

#include "controllPanel.h"
#include "ui_controllPanel.h"
#include "settingsDefine.h"



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

    for (unsigned i = 0; i < Color::Count; ++i)
    {
        auto color = colorToStr(static_cast<Color>(i));
        ui->interpolationPointColorCombo->addItem(color, i);
        ui->sensorPointColorCombo->addItem(color, i);
        ui->trueModelColorCombo->addItem(color, i);
    }

    for (unsigned i = 0; i < InterpolaionSpace::InterpolationType::Count; ++i)
        ui->interpolationCombo->addItem(InterpolaionSpace::getStrFromType(static_cast<InterpolaionSpace::InterpolationType>(i)), i);

    load();

    ui->startStopButton->setCheckable(true);

    connect(ui->sensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigSensorCountChanged);

    connect(ui->antennaLengthSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigAntennaLenghtChanged);

    connect(ui->interpolationPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationPointColorChanged);

    connect(ui->sensorPointColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleSensorPointColorChanged);

    connect(ui->trueModelCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigTrueModelCountChanged);

    connect(ui->trueModelColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleTrueModelColorChanged);

    connect(ui->trueModelPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigTrueModelPointSizeChanged);

    connect(ui->sensorPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigSensorPointSizeChanged);

    connect(ui->interpolationPointSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationPointSizeChanged);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &ControllPanel::handleEmulationButtonChange);

    connect(ui->interpolationCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationCountChanged);

    connect(ui->maximumDeviationSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigMaxDeviationChanged);

    connect(ui->antennaVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->sensorsVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->interpolationVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->interpolationCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationTypeChanged);
}

ControllPanel::~ControllPanel()
{
    save();
    delete ui;
}

void ControllPanel::load()
{
    using namespace BasicSettingValues;

    m_settings->beginGroup(SETTING_FIRST_GROUP);
        ui->sensorCountSpin->setValue(                    m_settings->value(SENSOR_COUNT,              BASIC_SENSOR_COUNT).toUInt());
        ui->antennaLengthSpin->setValue(                  m_settings->value(ANTENNA_LENGTH,            BASIC_ANTENNA_LENGHT).toDouble());
        ui->sensorPointSizeSpin->setValue(                m_settings->value(SENSOR_POINT_SIZE,         BASIC_SENSOR_POINT_SIZE).toDouble());
        ui->sensorPointColorCombo->setCurrentIndex(       m_settings->value(SENSOR_POINT_COLOR,        BASIC_SENSOR_POINT_COLOR).toUInt());
        ui->interpolationPointSizeSpin->setValue(         m_settings->value(INTERPOLATION_POINT_SIZE,  BASIC_INTERPOLATION_POINT_SIZE).toDouble());
        ui->interpolationPointColorCombo->setCurrentIndex(m_settings->value(INTERPOLATION_POINT_COLOR, BASIC_INTERPOLATION_POINT_COLOR).toUInt());
        ui->interpolationCountSpin->setValue(             m_settings->value(INTERPOLATION_COUNT,       BASIC_INTERPOLATION_COUNT).toUInt());
        ui->antennaVisibilityCheck->setChecked(           m_settings->value(ANTENNA_VISIBILITY,        BASIC_ANTENNA_VISIBILITY).toBool());
        ui->sensorsVisibilityCheck->setChecked(           m_settings->value(SENSOR_VISIBILITY,         BASIC_SENSOR_VISIBILITY).toBool());
        ui->interpolationVisibilityCheck->setChecked(     m_settings->value(INTERPOLATION_VISIBILITY,  BASIC_INTERPOLATION_VISIBILITY).toBool());
        ui->trueModelCountSpin->setValue(                 m_settings->value(MODEL_COUNT,               BASIC_MODEL_COUNT).toUInt());
        ui->trueModelPointSizeSpin->setValue(             m_settings->value(MODEL_POINT_SIZE,          BASIC_MODEL_POINT_SIZE).toDouble());
        ui->trueModelColorCombo->setCurrentIndex(         m_settings->value(MODEL_POINT_COLOR,         BASIC_MODEL_POINT_COLOR).toUInt());
        ui->interpolationCombo->setCurrentIndex(          m_settings->value(INTERPOLATION_TYPE,        BASIC_INTERPOLATION_TYPE).toUInt());
        ui->maximumDeviationSpin->setValue(               m_settings->value(MAX_DEVIATION,             0.1f).toDouble());
    m_settings->endGroup();
}

void ControllPanel::save()
{
    m_settings->beginGroup(SETTING_FIRST_GROUP);
        m_settings->setValue(SENSOR_COUNT,              ui->sensorCountSpin->value());
        m_settings->setValue(SENSOR_POINT_SIZE,         ui->sensorPointSizeSpin->value());
        m_settings->setValue(SENSOR_POINT_COLOR,        ui->sensorPointColorCombo->currentData());
        m_settings->setValue(INTERPOLATION_POINT_SIZE,  ui->interpolationPointSizeSpin->value());
        m_settings->setValue(INTERPOLATION_POINT_COLOR, ui->interpolationPointColorCombo->currentData());
        m_settings->setValue(INTERPOLATION_COUNT,       ui->interpolationCountSpin->value());
        m_settings->setValue(MAX_DEVIATION,             ui->maximumDeviationSpin->value());
        m_settings->setValue(ANTENNA_LENGTH,            ui->antennaLengthSpin->value());
        m_settings->setValue(ANTENNA_VISIBILITY,        ui->antennaVisibilityCheck->checkState());
        m_settings->setValue(SENSOR_VISIBILITY,         ui->sensorsVisibilityCheck->checkState());
        m_settings->setValue(INTERPOLATION_VISIBILITY,  ui->interpolationVisibilityCheck->checkState());
        m_settings->setValue(MODEL_COUNT,               ui->trueModelCountSpin->value());
        m_settings->setValue(MODEL_POINT_SIZE,          ui->trueModelPointSizeSpin->value());
        m_settings->setValue(MODEL_POINT_COLOR,         ui->trueModelColorCombo->currentData());
        m_settings->setValue(INTERPOLATION_TYPE,        ui->interpolationCombo->currentIndex());
    m_settings->endGroup();
}

double ControllPanel::getAntennaLenght() const
{
    return ui->antennaLengthSpin->value();
}

quint32 ControllPanel::getSensorCount() const
{
    return ui->sensorCountSpin->value();
}

quint32 ControllPanel::getInterpolationCount() const
{
    return ui->interpolationCountSpin->value();
}

quint32 ControllPanel::getTrueModelCount() const
{
    return ui->trueModelCountSpin->value();
}

InterpolaionSpace::InterpolationType ControllPanel::getInterpolationType() const
{
    return static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt());
}

double ControllPanel::getSensorSize() const
{
    return ui->sensorPointSizeSpin->value();
}

double ControllPanel::getInterpolationSize() const
{
    return ui->interpolationPointSizeSpin->value();
}

double ControllPanel::getTrueModelSize() const
{
    return ui->trueModelPointSizeSpin->value();
}

QColor ControllPanel::getSensorColor() const
{
    return QColor(ui->sensorPointColorCombo->currentText());
}

QColor ControllPanel::getInterpolationColor() const
{
    return QColor(ui->interpolationPointColorCombo->currentText());
}

QColor ControllPanel::getTrueModelColor() const
{
    return QColor(ui->trueModelColorCombo->currentText());
}

int ControllPanel::getAntennaVisibility() const
{
    return ui->antennaVisibilityCheck->checkState();
}

int ControllPanel::getSensorVisibility() const
{
    return ui->sensorsVisibilityCheck->checkState();
}

int ControllPanel::getInterpolationVisibility() const
{
    return ui->interpolationVisibilityCheck->checkState();
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

void ControllPanel::handleTrueModelColorChanged(const int &index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->trueModelColorCombo->currentText());
    emit sigTrueModelPointColorChanged(newColor);
}

void ControllPanel::handleEmulationButtonChange(const bool& checked)
{
    checked ? ui->startStopButton->setText(tr("Stop")) : ui->startStopButton->setText(tr("Start"));
    emit sigEmulationButtonClicked(checked);
}

void ControllPanel::handleVisibilityCheckBoxsChanged(const bool &checkState)
{
    const QString senderName = sender()->objectName();

    if (ui->antennaVisibilityCheck->objectName() == senderName)
        emit sigAntennaVisibilityChanged(checkState);
    if (ui->sensorsVisibilityCheck->objectName() == senderName)
        emit sigSensorVisibilityChanged(checkState);
    if (ui->interpolationVisibilityCheck->objectName() == senderName)
        emit sigInterpolationVisibilityChanged(checkState);
}

void ControllPanel::handleInterpolationTypeChanged(const int &index)
{
    Q_UNUSED(index);
    emit sigInterpolationTypeChanged(static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt()));
}
