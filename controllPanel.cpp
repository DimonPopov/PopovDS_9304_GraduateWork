#include <QSettings>

#include "controllPanel.h"
#include "ui_controllPanel.h"
#include "settingsDefine.h"



enum Color {
    Undefine = -1,
    Red,
    Green,
    Blue,
    Gold,
    Hotpink,
    Orange,
    Lime,
    Salmon,
    Count
};

QString colorToStr(const Color& color)
{
    switch (color)
    {
        case Red:     return "red";
        case Green:   return "green";
        case Gold:    return "gold";
        case Hotpink: return "hotpink";
        case Orange:  return "orange";
        case Lime:    return "lime";
        case Salmon:  return "salmon";
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
        const auto color = colorToStr(static_cast<Color>(i));
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(color));
        const QIcon icon(pixmap);
        ui->acousticSensorColorCombo->addItem(icon, color, i);
        ui->positionSensorColorCombo->addItem(icon, color, i);
        ui->modelColorCombo->addItem(icon, color, i);
    }

    for (unsigned i = 0; i < InterpolaionSpace::InterpolationType::Count; ++i)
        ui->interpolationCombo->addItem(InterpolaionSpace::getStrFromType(static_cast<InterpolaionSpace::InterpolationType>(i)), i);

    load();

    ui->startStopButton->setCheckable(true);

    connect(ui->positionSensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigSensorCountChanged);

    connect(ui->modelLengthSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigAntennaLenghtChanged);

    connect(ui->acousticSensorColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationPointColorChanged);

    connect(ui->positionSensorColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleSensorPointColorChanged);

    connect(ui->modelCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigTrueModelCountChanged);

    connect(ui->modelColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleTrueModelColorChanged);

    connect(ui->modelSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigTrueModelPointSizeChanged);

    connect(ui->positionSensorSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigSensorPointSizeChanged);

    connect(ui->acousticSensorSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationPointSizeChanged);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &ControllPanel::handleEmulationButtonChange);

    connect(ui->acousticSensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigInterpolationCountChanged);

    connect(ui->maximumDeviationSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigMaxDeviationChanged);

    connect(ui->modelVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->positionSensorVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->positionSensorEnd, &QCheckBox::stateChanged,
            this, &ControllPanel::sigSensorEndChanged);

    connect(ui->acousticSensorVisibilityCheck, &QCheckBox::stateChanged,
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
    ui->modelLengthSpin->setValue(                m_settings->value(MODEL_LENGTH,               BASIC_MODEL_LENGHT).toDouble());
    ui->modelCountSpin->setValue(                 m_settings->value(MODEL_COUNT,                BASIC_MODEL_COUNT).toUInt());
    ui->modelColorCombo->setCurrentIndex(         m_settings->value(MODEL_COLOR,                BASIC_MODEL_COLOR).toUInt());
    ui->modelSizeSpin->setValue(                  m_settings->value(MODEL_SIZE,                 BASIC_MODEL_SIZE).toDouble());
    ui->modelVisibilityCheck->setChecked(         m_settings->value(MODEL_VISIBILITY,           BASIC_MODEL_VISIBILITY).toBool());
    ui->positionSensorCountSpin->setValue(        m_settings->value(POSITION_SENSOR_COUNT,      BASIC_POSITION_SENSOR_COUNT).toUInt());
    ui->positionSensorColorCombo->setCurrentIndex(m_settings->value(POSITION_SENSOR_COLOR,      BASIC_POSITION_SENSOR_COLOR).toUInt());
    ui->positionSensorSizeSpin->setValue(         m_settings->value(POSITION_SENSOR_SIZE,       BASIC_POSITION_SENSOR_SIZE).toDouble());
    ui->positionSensorVisibilityCheck->setChecked(m_settings->value(POSITION_SENSOR_VISIBILITY, BASIC_POSITION_SENSOR_VISIBILITY).toBool());
    ui->positionSensorEnd->setChecked(            m_settings->value(POSITION_SENSOR_END,        BASIC_POSITION_SENSOR_END).toBool());
    ui->acousticSensorCountSpin->setValue(        m_settings->value(ACOUSTIC_SENSOR_COUNT,      BASIC_ACOUSTIC_SENSOR_COUNT).toUInt());
    ui->acousticSensorColorCombo->setCurrentIndex(m_settings->value(ACOUSTIC_SENSOR_COLOR,      BASIC_ACOUSTIC_SENSOR_COLOR).toUInt());
    ui->acousticSensorSizeSpin->setValue(         m_settings->value(ACOUSTIC_SENSOR_SIZE,       BASIC_ACOUSTIC_SENSOR_SIZE).toDouble());
    ui->acousticSensorVisibilityCheck->setChecked(m_settings->value(ACOUSTIC_SENSOR_VISIBILITY, BASIC_ACOUSTIC_SENSOR_VISIBILITY).toBool());
        ui->interpolationCombo->setCurrentIndex(  m_settings->value(INTERPOLATION_TYPE,         BASIC_INTERPOLATION_TYPE).toUInt());

        ui->maximumDeviationSpin->setValue(               m_settings->value(MAX_DEVIATION,             0.1f).toDouble());
    m_settings->endGroup();
}

void ControllPanel::save()
{
    m_settings->beginGroup(SETTING_FIRST_GROUP);
    m_settings->setValue(MODEL_LENGTH,               ui->modelLengthSpin->value());
    m_settings->setValue(MODEL_COUNT,                ui->modelCountSpin->value());
    m_settings->setValue(MODEL_COLOR,                ui->modelColorCombo->currentData());
    m_settings->setValue(MODEL_SIZE,                 ui->modelSizeSpin->value());
    m_settings->setValue(MODEL_VISIBILITY,           ui->modelVisibilityCheck->checkState());
    m_settings->setValue(POSITION_SENSOR_COUNT,      ui->positionSensorCountSpin->value());
    m_settings->setValue(POSITION_SENSOR_COLOR,      ui->positionSensorColorCombo->currentData());
    m_settings->setValue(POSITION_SENSOR_SIZE,       ui->positionSensorSizeSpin->value());
    m_settings->setValue(POSITION_SENSOR_VISIBILITY, ui->positionSensorVisibilityCheck->checkState());
    m_settings->setValue(POSITION_SENSOR_END,        ui->positionSensorEnd->checkState());
    m_settings->setValue(ACOUSTIC_SENSOR_COUNT,      ui->acousticSensorCountSpin->value());
    m_settings->setValue(ACOUSTIC_SENSOR_COLOR,      ui->acousticSensorColorCombo->currentData());
    m_settings->setValue(ACOUSTIC_SENSOR_SIZE,       ui->acousticSensorSizeSpin->value());
    m_settings->setValue(ACOUSTIC_SENSOR_VISIBILITY, ui->acousticSensorVisibilityCheck->checkState());
        m_settings->setValue(INTERPOLATION_TYPE,         ui->interpolationCombo->currentIndex());

        m_settings->setValue(MAX_DEVIATION,              ui->maximumDeviationSpin->value());
    m_settings->endGroup();
}

double ControllPanel::getAntennaLenght() const
{
    return ui->modelLengthSpin->value();
}

quint32 ControllPanel::getSensorCount() const
{
    return ui->positionSensorCountSpin->value();
}

quint32 ControllPanel::getInterpolationCount() const
{
    return ui->acousticSensorCountSpin->value();
}

quint32 ControllPanel::getTrueModelCount() const
{
    return ui->modelCountSpin->value();
}

InterpolaionSpace::InterpolationType ControllPanel::getInterpolationType() const
{
    return static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt());
}

double ControllPanel::getSensorSize() const
{
    return ui->positionSensorSizeSpin->value();
}

double ControllPanel::getInterpolationSize() const
{
    return ui->acousticSensorSizeSpin->value();
}

double ControllPanel::getTrueModelSize() const
{
    return ui->modelSizeSpin->value();
}

QColor ControllPanel::getSensorColor() const
{
    return QColor(ui->positionSensorColorCombo->currentText());
}

QColor ControllPanel::getInterpolationColor() const
{
    return QColor(ui->acousticSensorColorCombo->currentText());
}

QColor ControllPanel::getTrueModelColor() const
{
    return QColor(ui->modelColorCombo->currentText());
}

int ControllPanel::getAntennaVisibility() const
{
    return ui->modelVisibilityCheck->checkState();
}

int ControllPanel::getSensorVisibility() const
{
    return ui->positionSensorVisibilityCheck->checkState();
}

int ControllPanel::getInterpolationVisibility() const
{
    return ui->acousticSensorVisibilityCheck->checkState();
}

bool ControllPanel::getSensorEnd() const
{
    return ui->positionSensorEnd->checkState();
}

void ControllPanel::handleSensorPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->positionSensorColorCombo->currentText());
    emit sigSensorPointColorChanged(newColor);
}

void ControllPanel::handleInterpolationPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->acousticSensorColorCombo->currentText());
    emit sigInterpolationPointColorChanged(newColor);
}

void ControllPanel::handleTrueModelColorChanged(const int &index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->modelColorCombo->currentText());
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

    if (ui->modelVisibilityCheck->objectName() == senderName)
        emit sigAntennaVisibilityChanged(checkState);
    if (ui->positionSensorVisibilityCheck->objectName() == senderName)
        emit sigSensorVisibilityChanged(checkState);
    if (ui->acousticSensorVisibilityCheck->objectName() == senderName)
        emit sigInterpolationVisibilityChanged(checkState);
}

void ControllPanel::handleInterpolationTypeChanged(const int &index)
{
    Q_UNUSED(index);
    emit sigInterpolationTypeChanged(static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt()));
}
