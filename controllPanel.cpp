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

    connect(ui->intervalModelUpdateSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigIntervalModelChanged);

    connect(ui->stepModelUpdateSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigStepModelChanged);

    connect(ui->positionSensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigPositionSensorCountChanged);

    connect(ui->modelLengthSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigModelLenghtChanged);

    connect(ui->acousticSensorColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleAcousticSensorColorChanged);

    connect(ui->positionSensorColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handlePositionSensorColorChanged);

    connect(ui->modelCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigModelCountChanged);

    connect(ui->modelColorCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleModelColorChanged);

    connect(ui->modelSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigModelSizeChanged);

    connect(ui->positionSensorSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigPositionSensorSizeChanged);

    connect(ui->acousticSensorSizeSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigAcousticSensorSizeChanged);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &ControllPanel::handleEmulationButtonChange);

    connect(ui->acousticSensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::sigAcousticSensorCountChanged);

    connect(ui->modelVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->positionSensorVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->positionSensorEnd, &QCheckBox::stateChanged,
            this, &ControllPanel::sigPositionSensorEndChanged);

    connect(ui->acousticSensorVisibilityCheck, &QCheckBox::stateChanged,
            this, &ControllPanel::handleVisibilityCheckBoxsChanged);

    connect(ui->interpolationCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleInterpolationTypeChanged);

    connect(ui->maxNoiseX, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::handleNoiseChanged);

    connect(ui->maxNoiseY, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::handleNoiseChanged);

    connect(ui->maxNoiseZ, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::handleNoiseChanged);
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
        ui->interpolationCombo->setCurrentIndex(      m_settings->value(INTERPOLATION_TYPE,         BASIC_INTERPOLATION_TYPE).toUInt());

        ui->maxNoiseX->setValue(                      m_settings->value(MAX_NOISE_X,                BASIC_MAX_NOISE_X).toDouble());
        ui->maxNoiseY->setValue(                      m_settings->value(MAX_NOISE_Y,                BASIC_MAX_NOISE_Y).toDouble());
        ui->maxNoiseZ->setValue(                      m_settings->value(MAX_NOISE_Z,                BASIC_MAX_NOISE_Z).toDouble());
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

        m_settings->setValue(MAX_NOISE_X,                ui->maxNoiseX->value());
        m_settings->setValue(MAX_NOISE_Y,                ui->maxNoiseY->value());
        m_settings->setValue(MAX_NOISE_Z,                ui->maxNoiseZ->value());
    m_settings->endGroup();
}

double ControllPanel::getModelLenght() const
{
    return ui->modelLengthSpin->value();
}

quint32 ControllPanel::getPositionSensorCount() const
{
    return ui->positionSensorCountSpin->value();
}

quint32 ControllPanel::getAcousticSensorCount() const
{
    return ui->acousticSensorCountSpin->value();
}

quint32 ControllPanel::getModelCount() const
{
    return ui->modelCountSpin->value();
}

InterpolaionSpace::InterpolationType ControllPanel::getInterpolationType() const
{
    return static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt());
}

double ControllPanel::getPositionSensorSize() const
{
    return ui->positionSensorSizeSpin->value();
}

double ControllPanel::getAcousticSensorSize() const
{
    return ui->acousticSensorSizeSpin->value();
}

double ControllPanel::getModelSize() const
{
    return ui->modelSizeSpin->value();
}

QColor ControllPanel::getPositionSensorColor() const
{
    return QColor(ui->positionSensorColorCombo->currentText());
}

QColor ControllPanel::getAcousticSensorColor() const
{
    return QColor(ui->acousticSensorColorCombo->currentText());
}

QColor ControllPanel::getModelColor() const
{
    return QColor(ui->modelColorCombo->currentText());
}

double ControllPanel::getModelInterval() const
{
    return ui->intervalModelUpdateSpin->value();
}

double ControllPanel::getModelStep() const
{
    return ui->stepModelUpdateSpin->value();
}

int ControllPanel::getModelVisibility() const
{
    return ui->modelVisibilityCheck->checkState();
}

int ControllPanel::getPositionSensorVisibility() const
{
    return ui->positionSensorVisibilityCheck->checkState();
}

int ControllPanel::getAcousticSensorVisibility() const
{
    return ui->acousticSensorVisibilityCheck->checkState();
}

QVector3D ControllPanel::getMaxNoise() const
{
    return QVector3D(ui->maxNoiseX->value() / 2,
                     ui->maxNoiseY->value() / 2,
                     ui->maxNoiseZ->value() / 2);
}

bool ControllPanel::getPositionSensorEnd() const
{
    return ui->positionSensorEnd->checkState();
}

void ControllPanel::handlePositionSensorColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->positionSensorColorCombo->currentText());
    emit sigPositionSensorColorChanged(newColor);
}

void ControllPanel::handleAcousticSensorColorChanged(const int& index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->acousticSensorColorCombo->currentText());
    emit sigAcousticSensorColorChanged(newColor);
}

void ControllPanel::handleModelColorChanged(const int &index)
{
    Q_UNUSED(index);
    const QColor newColor = QColor(ui->modelColorCombo->currentText());
    emit sigModelColorChanged(newColor);
}

void ControllPanel::handleEmulationButtonChange(const bool& checked)
{
    checked ? ui->startStopButton->setText(tr("Stop")) : ui->startStopButton->setText(tr("Start"));
    emit sigEmulationButtonClicked(checked);
}

void ControllPanel::handleNoiseChanged(const double &value)
{
    Q_UNUSED(value);
    emit sigMaxNoiseChanged(QVector3D(ui->maxNoiseX->value() / 2,
                                      ui->maxNoiseY->value() / 2,
                                      ui->maxNoiseZ->value() / 2));
}

void ControllPanel::handleVisibilityCheckBoxsChanged(const bool &checkState)
{
    const QString senderName = sender()->objectName();

    if (ui->modelVisibilityCheck->objectName() == senderName)
        emit sigModelVisibilityChanged(checkState);
    if (ui->positionSensorVisibilityCheck->objectName() == senderName)
        emit sigPositionSensorVisibilityChanged(checkState);
    if (ui->acousticSensorVisibilityCheck->objectName() == senderName)
        emit sigAcousticSensorVisibilityChanged(checkState);
}

void ControllPanel::handleInterpolationTypeChanged(const int &index)
{
    Q_UNUSED(index);
    emit sigInterpolationTypeChanged(static_cast<InterpolaionSpace::InterpolationType>(ui->interpolationCombo->currentData().toUInt()));
}
