#include "mainwindow.h"
#include "surfacegraph.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QComboBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Q3DSurface *graph = new Q3DSurface;
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Couldn't initialize the OpenGL context."));
        msgBox.exec();
        return;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    widget->setWindowTitle(tr("Surface"));

    QGroupBox *selectionGroupBox = new QGroupBox(tr("Selection Mode"));

    QRadioButton *modeNoneRB = new QRadioButton(widget);
    modeNoneRB->setText(tr("No selection"));
    modeNoneRB->setChecked(false);

    QRadioButton *modeItemRB = new QRadioButton(widget);
    modeItemRB->setText(tr("Item"));
    modeItemRB->setChecked(false);

    QRadioButton *modeSliceRowRB = new QRadioButton(widget);
    modeSliceRowRB->setText(tr("Row Slice"));
    modeSliceRowRB->setChecked(false);

    QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
    modeSliceColumnRB->setText(tr("Column Slice"));
    modeSliceColumnRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));

    vLayout->addWidget(selectionGroupBox);
    vLayout->addWidget(new QLabel(tr("Column range")));
    vLayout->addWidget(axisMinSliderX);
    vLayout->addWidget(axisMaxSliderX);
    vLayout->addWidget(new QLabel(tr("Row range")));
    vLayout->addWidget(axisMinSliderZ);
    vLayout->addWidget(axisMaxSliderZ);
    vLayout->addWidget(new QLabel(tr("Theme")));
    vLayout->addWidget(themeList);

    SurfaceGraph *modifier = new SurfaceGraph(graph);

    connect(modeNoneRB, &QRadioButton::toggled,
            modifier, &SurfaceGraph::toggleModeNone);
    connect(modeItemRB, &QRadioButton::toggled,
            modifier, &SurfaceGraph::toggleModeItem);
    connect(modeSliceRowRB, &QRadioButton::toggled,
            modifier, &SurfaceGraph::toggleModeSliceRow);
    connect(modeSliceColumnRB, &QRadioButton::toggled,
            modifier, &SurfaceGraph::toggleModeSliceColumn);
    connect(axisMinSliderX, &QSlider::valueChanged,
            modifier, &SurfaceGraph::adjustXMin);
    connect(axisMaxSliderX, &QSlider::valueChanged,
            modifier, &SurfaceGraph::adjustXMax);
    connect(axisMinSliderZ, &QSlider::valueChanged,
            modifier, &SurfaceGraph::adjustZMin);
    connect(axisMaxSliderZ, &QSlider::valueChanged,
            modifier, &SurfaceGraph::adjustZMax);
    connect(themeList, QOverload<int>::of(&QComboBox::currentIndexChanged),
            modifier, &SurfaceGraph::changeTheme);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);

    setCentralWidget(widget);

    modifier->enableSqrtSinModel(true);
}



MainWindow::~MainWindow()
{
    delete ui;
}

