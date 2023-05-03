#include <QSettings>
#include <QFileDialog>

#include "controllPanel.h"
#include "ui_controllPanel.h"
#include "settingsDefine.h"
//#include "modelsettingsdialog.h"



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
        case Red:     return QObject::tr("red");
        case Green:   return QObject::tr("green");
        case Gold:    return QObject::tr("gold");
        case Hotpink: return QObject::tr("hotpink");
        case Orange:  return QObject::tr("orange");
        case Lime:    return QObject::tr("lime");
        case Salmon:  return QObject::tr("salmon");
        case Blue:
        default:    return QObject::tr("blue");
    }
}

enum Model {
    First,
    Second,
    Third,
    ModelCount
};

QString modelToStr(const Model& model)
{
    switch (model)
    {
    case First:    return "2 + sin(x)";
    case Second:   return "3 + sin(x) * 0.4";
    case Third:    return "2 + sin(x) * 3";

    default:    return "";
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

    for (unsigned i = 0; i < ModelCount; ++i)
        ui->modelCombo->addItem(modelToStr(static_cast<Model>(i)), i);

    load();

    ui->startStopButton->setCheckable(true);
    ui->languageSetButton->setCheckable(true);
    handleMaxValueNoiseChange();

    connect(ui->openFileButton, &QPushButton::clicked,
            this, &ControllPanel::handleOpenFileDialog);

    connect(ui->languageSetButton, &QPushButton::clicked,
            this, &ControllPanel::handleLanguageChanged);

    connect(ui->modelCombo, &QComboBox::currentIndexChanged,
            this, &ControllPanel::handleModelChanged);

    connect(ui->noiseBox, &QGroupBox::clicked,
            this, &ControllPanel::sigNoiseChanged);

    connect(ui->intervalModelUpdateSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigIntervalModelChanged);

    connect(ui->stepModelUpdateSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::sigStepModelChanged);

    connect(ui->positionSensorCountSpin, &QSpinBox::valueChanged,
            this, &ControllPanel::handleMaxValueNoiseChange);

    connect(ui->modelLengthSpin, &QDoubleSpinBox::valueChanged,
            this, &ControllPanel::handleMaxValueNoiseChange);

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

    connect(ui->truePositionRadio, &QRadioButton::toggled,
            this, &ControllPanel::sigDisplayOptionChanged);
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
        ui->intervalModelUpdateSpin->setValue(        m_settings->value(MODEL_INTERVAL,             BASIC_MODEL_INTERVAL).toDouble());
        ui->stepModelUpdateSpin->setValue(            m_settings->value(MODEL_STEP,                 BASIC_MODEL_STEP).toDouble());
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
        m_settings->setValue(MODEL_INTERVAL,             ui->intervalModelUpdateSpin->value());
        m_settings->setValue(MODEL_STEP,                 ui->stepModelUpdateSpin->value());
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

void ControllPanel::handleUpdateInterpolationTime(const quint32 &time)
{
    ui->interpolationTimeSpin->setValue(time);
}

void ControllPanel::handleMaxValueNoiseChange()
{
    ui->maxNoiseX->setMaximum((ui->modelLengthSpin->value() / ui->positionSensorCountSpin->value()) / 2);
}

//void ControllPanel::handleOpenModelSettingsDialog()
//{
//    ModelSettingsDialog dialog;
//    dialog.exec();
//}

void ControllPanel::handleOpenFileDialog()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.exec();
}

void ControllPanel::handleModelChanged(const int& index)
{
    Q_UNUSED(index);
    auto text = ui->modelCombo->currentText();
    if (text == modelToStr(First))
        emit sigModelChanged({2, 1});
    else if (text == modelToStr(Second))
        emit sigModelChanged({3, 0.4});
    else if (text == modelToStr(Third))
        emit sigModelChanged({2, 3});
}

void ControllPanel::handleLanguageChanged(const bool& checked)
{
    if (checked)
    {
        ui->languageSetButton->setIcon(QIcon(":/img/images/russia.png"));
        ui->LenghtLabel->setText("Длина:");
        ui->ModelLabel->setText("Модель:");
        ui->PointSettingsLabel->setText("Настройки точек:");
        ui->pointsSettingTab->setTabText(0, "Модель");
        ui->pointsSettingTab->setTabText(1, "Известные");
        ui->pointsSettingTab->setTabText(2, "Востановл.");
        ui->pointsSettingTab->setTabToolTip(0, "Модель");
        ui->pointsSettingTab->setTabToolTip(1, "Известные точки");
        ui->pointsSettingTab->setTabToolTip(2, "Востановленные точки");
        ui->AmountLabel->setText("Кол-во:");
        ui->AmountLabel_2->setText("Кол-во:");
        ui->AmountLabel_3->setText("Кол-во:");
        ui->SizeLabel->setText("Размер:");
        ui->SizeLabel_2->setText("Размер:");
        ui->SizeLabel_3->setText("Размер:");
        ui->ColorLabel->setText("Цвет:");
        ui->ColorLabel_2->setText("Цвет:");
        ui->ColorLabel_3->setText("Цвет:");
        ui->modelVisibilityCheck->setText("Отображать");
        ui->acousticSensorVisibilityCheck->setText("Отображать");
        ui->positionSensorVisibilityCheck->setText("Отображать");
        ui->positionSensorEnd->setText("Датчик в конце");
        ui->noiseBox->setTitle("Шумы");
        ui->MaxXLabel->setText("Макс. X: ");
        ui->MaxYLabel->setText("Макс. Y: ");
        ui->MaxZLabel->setText("Макс. Z: ");
        ui->TimeLabel->setText("Время:");
        ui->truePositionRadio->setText("Истинная поз.");
        ui->absDeviationRadio->setText("Модуль откл.");
        ui->DisplayLabel->setText("Парам.");
        ui->OptionLabel->setText("отобр.:");
        ui->interpolationTimeSpin->setSuffix(" микроСек");
        ui->IntervalLabel->setText("Интер.:");
        ui->intervalModelUpdateSpin->setSuffix(" Сек");
        ui->StepLabel->setText("Шаг:");
        ui->startStopButton->setText(ui->startStopButton->isChecked() ? "Стоп" : "Старт");
    }
    else
    {
        ui->languageSetButton->setIcon(QIcon(":/img/images/united-kingdom.png"));
        ui->LenghtLabel->setText("Length:");
        ui->ModelLabel->setText("Model:");
        ui->PointSettingsLabel->setText("Point settings:");
        ui->pointsSettingTab->setTabText(0, "Model");
        ui->pointsSettingTab->setTabText(1, "Position");
        ui->pointsSettingTab->setTabText(2, "Acoustic");
        ui->pointsSettingTab->setTabToolTip(0, "Model");
        ui->pointsSettingTab->setTabToolTip(1, "Position");
        ui->pointsSettingTab->setTabToolTip(2, "Acoustic");
        ui->AmountLabel->setText("Amount:");
        ui->AmountLabel_2->setText("Amount:");
        ui->AmountLabel_3->setText("Amount:");
        ui->SizeLabel->setText("Size:");
        ui->SizeLabel_2->setText("Size:");
        ui->SizeLabel_3->setText("Size:");
        ui->ColorLabel->setText("Color:");
        ui->ColorLabel_2->setText("Color:");
        ui->ColorLabel_3->setText("Color:");
        ui->modelVisibilityCheck->setText("Display model");
        ui->acousticSensorVisibilityCheck->setText("Display acoustic sensors");
        ui->positionSensorVisibilityCheck->setText("Display position sensors");
        ui->positionSensorEnd->setText("Sensor at the end");
        ui->noiseBox->setTitle("Noise");
        ui->MaxXLabel->setText("Max X: ");
        ui->MaxYLabel->setText("Max Y: ");
        ui->MaxZLabel->setText("Max Z: ");
        ui->TimeLabel->setText("Time:");
        ui->truePositionRadio->setText("True position");
        ui->absDeviationRadio->setText("Abs deviation");
        ui->DisplayLabel->setText("Display");
        ui->OptionLabel->setText("option:");
        ui->interpolationTimeSpin->setSuffix(" microSec");
        ui->IntervalLabel->setText("Interval:");
        ui->intervalModelUpdateSpin->setSuffix(" Sec");
        ui->StepLabel->setText("Step:");
        ui->startStopButton->setText(ui->startStopButton->isChecked() ? "Stop" : "Start");
    }
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
    return QVector3D(ui->maxNoiseX->value(),
                     ui->maxNoiseY->value(),
                     ui->maxNoiseZ->value());
}

QPair<double, double> ControllPanel::getModel() const
{
    auto text = ui->modelCombo->currentText();
    if (text == modelToStr(First))
        return {2, 1};
    else if (text == modelToStr(Second))
        return {3, 0.4};
    else if (text == modelToStr(Third))
        return {4, 0.7};

    return {2, 1};
}

bool ControllPanel::getPositionSensorEnd() const
{
    return ui->positionSensorEnd->checkState();
}

bool ControllPanel::getPositionSensorNoise() const
{
    return ui->noiseBox->isChecked();
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
    bool check = ui->languageSetButton->isChecked();
    if (check)
        checked ? ui->startStopButton->setText("Стоп") : ui->startStopButton->setText("Старт");
    else
        checked ? ui->startStopButton->setText("Stop") : ui->startStopButton->setText("Start");
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
