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



using namespace PointContainerSpace;
using namespace AntennaModelSpace;

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

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();

    m_controllPanel = new ControllPanel(this);

    QSharedPointer<AntennaModel> antennaModel(new AntennaModel(m_controllPanel->getAntennaLenght()));
    QSharedPointer<TrueModel> trueModel(new TrueModel(antennaModel,
                                                      m_controllPanel->getTrueModelCount()));
    QSharedPointer<PositionSensors> positionSensors(new PositionSensors(antennaModel,
                                                                        m_controllPanel->getSensorCount(),
                                                                        m_controllPanel->getSensorEnd()));
    QSharedPointer<AcousticSensors> acousticSensors(new AcousticSensors(antennaModel,
                                                                        positionSensors,
                                                                        m_controllPanel->getInterpolationType(),
                                                                        m_controllPanel->getInterpolationCount()));

    m_graph = new ScatterGraph(graph, positionSensors, acousticSensors, trueModel);    

    m_graph->handleSetSensorSize(m_controllPanel->getSensorSize());
    m_graph->handleSetInterpolationSize(m_controllPanel->getInterpolationSize());
    m_graph->handleSetSensorColor(m_controllPanel->getSensorColor());
    m_graph->handleSetInterpolationColor(m_controllPanel->getInterpolationColor());
    m_graph->handleSetAntennaVisibility(m_controllPanel->getAntennaVisibility());
    m_graph->handleSetSensorVisibility(m_controllPanel->getSensorVisibility());
    m_graph->handleSetInterpolationVisibility(m_controllPanel->getInterpolationVisibility());
    m_graph->handleSetTrueModelColor(m_controllPanel->getTrueModelColor());
    m_graph->handleSetTrueModelSize(m_controllPanel->getTrueModelSize());

    connect(m_controllPanel, &ControllPanel::sigSensorCountChanged,
            positionSensors.data(), &PositionSensors::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigInterpolationCountChanged,
            acousticSensors.data(), &AcousticSensors::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigTrueModelCountChanged,
            trueModel.data(), &TrueModel::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigAntennaLenghtChanged,
            antennaModel.data(), &AntennaModel::setLenght);

    connect(m_controllPanel, &ControllPanel::sigEmulationButtonClicked,
            positionSensors.data(), &PositionSensors::handleSetNoiseState);

    connect(m_controllPanel, &ControllPanel::sigSensorEndChanged,
            positionSensors.data(), &PositionSensors::handleSetSensorEnd);

    connect(m_controllPanel, &ControllPanel::sigInterpolationTypeChanged,
            acousticSensors.data(), &AcousticSensors::setInterpolationType);

    connect(m_controllPanel, &ControllPanel::sigTrueModelPointSizeChanged,
            m_graph, &ScatterGraph::handleSetTrueModelSize);

    connect(m_controllPanel, &ControllPanel::sigTrueModelPointColorChanged,
            m_graph, &ScatterGraph::handleSetTrueModelColor);

    connect(m_controllPanel, &ControllPanel::sigSensorPointSizeChanged,
            m_graph, &ScatterGraph::handleSetSensorSize);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointSizeChanged,
            m_graph, &ScatterGraph::handleSetInterpolationSize);

    connect(m_controllPanel, &ControllPanel::sigSensorPointColorChanged,
            m_graph, &ScatterGraph::handleSetSensorColor);

    connect(m_controllPanel, &ControllPanel::sigInterpolationPointColorChanged,
            m_graph, &ScatterGraph::handleSetInterpolationColor);

    connect(m_controllPanel, &ControllPanel::sigAntennaVisibilityChanged,
            m_graph, &ScatterGraph::handleSetAntennaVisibility);

    connect(m_controllPanel, &ControllPanel::sigSensorVisibilityChanged,
            m_graph, &ScatterGraph::handleSetSensorVisibility);

    connect(m_controllPanel, &ControllPanel::sigInterpolationVisibilityChanged,
            m_graph, &ScatterGraph::handleSetInterpolationVisibility);

    QSize screenSize = graph->screen()->size();
    QWidget *container = QWidget::createWindowContainer(graph, m_graph);

    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

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

