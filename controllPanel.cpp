#include "controllPanel.h"
#include "ui_controllPanel.h"

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
        case Blue:  return "blue";
        default:    return "black";
    }
}



ControllPanel::ControllPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControllPanel)
{
    ui->setupUi(this);

    for (int i = 0; i < Color::Count; ++i)
    {
        ui->interpolationPointColorCombo->addItem(colorToStr(static_cast<Color>(i)));
        ui->sensorPointColorCombo->addItem(colorToStr(static_cast<Color>(i)));
    }

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
}

bool ControllPanel::load()
{
    return true;
}

bool ControllPanel::save()
{
    return true;
}

ControllPanel::~ControllPanel()
{
    delete ui;
}

void ControllPanel::handleSensorPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    QColor newColor = QColor(ui->sensorPointColorCombo->currentText());
    emit sigSensorPointColorChanged(newColor);
}

void ControllPanel::handleInterpolationPointColorChanged(const int& index)
{
    Q_UNUSED(index);
    QColor newColor = QColor(ui->interpolationPointColorCombo->currentText());
    emit sigInterpolationPointColorChanged(newColor);
}
