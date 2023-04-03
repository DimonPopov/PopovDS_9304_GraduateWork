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

    // Получение значений панели управления для графика.

    m_graph->handleSetSensorData({m_controllPanel->getSensorCount(), m_controllPanel->getAntennaLenght()});
    m_graph->handleSetInterpolationCount(m_controllPanel->getInterpolationCount());
    m_graph->handleSetSensorSize(m_controllPanel->getSensorSize());
    m_graph->handleSetInterpolationSize(m_controllPanel->getInterpolationSize());
    m_graph->handleSetSensorColor(m_controllPanel->getSensorColor());
    m_graph->handleSetInterpolationColor(m_controllPanel->getInterpolationColor());
    m_graph->handleSetAntennaVisibility(m_controllPanel->getAntennaVisibility());
    m_graph->handleSetSensorVisibility(m_controllPanel->getSensorVisibility());
    m_graph->handleSetInterpolationVisibility(m_controllPanel->getInterpolationVisibility());

    connect(m_controllPanel, &ControllPanel::sigSensorDataChanged,
            m_graph, &ScatterGraph::handleSetSensorData);

    connect(m_controllPanel, &ControllPanel::sigInterpolationCountChanged,
            m_graph, &ScatterGraph::handleSetInterpolationCount);

    connect(m_controllPanel, &ControllPanel::sigSensorPointSizeChanged,
            m_graph, &ScatterGraph::handleSetSensorSize);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointSizeChanged,
            m_graph, &ScatterGraph::handleSetInterpolationSize);

    connect(m_controllPanel, &ControllPanel::sigSensorPointColorChanged,
            m_graph, &ScatterGraph::handleSetSensorColor);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointColorChanged,
            m_graph, &ScatterGraph::handleSetInterpolationColor);

    connect(m_controllPanel, &ControllPanel::sigEmulationButtonClicked,
            m_graph, &ScatterGraph::handleSetEmulationState);

    connect(m_controllPanel, &ControllPanel::sigMaxDeviationChanged,
            m_graph, &ScatterGraph::handleSetMaxDeviation);

    connect(m_controllPanel, &ControllPanel::sigAntennaVisibilityChanged,
            m_graph, &ScatterGraph::handleSetAntennaVisibility);

    connect(m_controllPanel, &ControllPanel::sigSensorVisibilityChanged,
            m_graph, &ScatterGraph::handleSetSensorVisibility);

    connect(m_controllPanel, &ControllPanel::sigInterpolationVisibilityChanged,
            m_graph, &ScatterGraph::handleSetInterpolationVisibility);

//    m_graph->calculateInterpolation();

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

