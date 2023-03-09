#include "mainwindow.h"
#include "scatterGraph.h"
#include "ui_mainwindow.h"
#include "controllPanel.h"

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

    Q3DScatter *graph = new Q3DScatter;

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Couldn't initialize the OpenGL context."));
        msgBox.exec();
        return;
    }

    setWindowTitle(tr("Surface"));

    QSize screenSize = graph->screen()->size();
    QWidget *container = QWidget::createWindowContainer(graph);

    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();

    m_controllPanel = new ControllPanel(this);
    m_graph = new ScatterGraph(graph);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointColorChanged,
            m_graph, &ScatterGraph::handleSetInterpolationColor);

    connect(m_controllPanel, &ControllPanel::sigSensorPointColorChanged,
            m_graph, &ScatterGraph::handleSetSensorColor);

    connect(m_controllPanel, &ControllPanel::sigSensorPointSizeChanged,
            m_graph, &ScatterGraph::handleSetSensorSize);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointSizeChanged,
            m_graph, &ScatterGraph::handleSetInterpolationSize);

    connect(m_controllPanel, &ControllPanel::sigSensorCountChanged,
            m_graph, &ScatterGraph::handleSetSensorCount);

    connect(m_controllPanel, &ControllPanel::sigEmulationButtonClicked,
            m_graph, &ScatterGraph::handleSetEmulationState);

    m_controllPanel->emitAllSignal();

    hLayout->addWidget(container, 1);
    vLayout->addWidget(m_controllPanel);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);


    setCentralWidget(widget);
}



MainWindow::~MainWindow()
{
    delete ui;
    delete m_graph;
}

