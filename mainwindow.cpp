#include "mainwindow.h"
#include "scatterGraph.h"
#include "ui_mainwindow.h"
#include "controllPanel.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>



using namespace PointContainerSpace;
using namespace AntennaModelSpace;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    QTranslator translator;
    //    const QStringList uiLanguages = QLocale::system().uiLanguages();
    //    for (const QString &locale : uiLanguages) {
    //        const QString baseName = "GraduateWork_" + QLocale(locale).name();
    //        if (translator.load(":/i18n/" + baseName)) {
    //            a.installTranslator(&translator);
    //            break;
    //        }
    //    }

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

    QSharedPointer<AntennaModel> antennaModel(new AntennaModel(m_controllPanel->getModelLenght(),
                                                               0,
                                                               m_controllPanel->getModelStep(),
                                                               m_controllPanel->getModelInterval(),
                                                               m_controllPanel->getModel(),
                                                               m_controllPanel->getMaxNoise()));

    QSharedPointer<TrueModel> trueModel(new TrueModel(antennaModel,
                                                      m_controllPanel->getModelCount()));

    QSharedPointer<PositionSensors> positionSensors(new PositionSensors(antennaModel,
                                                                        m_controllPanel->getPositionSensorCount(),
                                                                        m_controllPanel->getPositionSensorEnd(),
                                                                        m_controllPanel->getPositionSensorNoise()));

    QSharedPointer<AcousticSensors> acousticSensors(new AcousticSensors(antennaModel,
                                                                        positionSensors,
                                                                        m_controllPanel->getInterpolationType(),
                                                                        m_controllPanel->getAcousticSensorCount()));

    m_graph = new ScatterGraph(graph, positionSensors, acousticSensors, trueModel);

    m_graph->handleSetPositionSensorSize(m_controllPanel->getPositionSensorSize());
    m_graph->handleSetPositionSensorColor(m_controllPanel->getPositionSensorColor());
    m_graph->handleSetPositionSensorVisibility(m_controllPanel->getPositionSensorVisibility());
    m_graph->handleSetAcousticSensorSize(m_controllPanel->getAcousticSensorSize());
    m_graph->handleSetAcousticSensorColor(m_controllPanel->getAcousticSensorColor());
    m_graph->handleSetAcousticSensorVisibility(m_controllPanel->getAcousticSensorVisibility());
    m_graph->handleSetTrueModelColor(m_controllPanel->getModelColor());
    m_graph->handleSetTrueModelSize(m_controllPanel->getModelSize());
    m_graph->handleSetTrueModelVisibility(m_controllPanel->getModelVisibility());

    m_controllPanel->handleUpdateInterpolationTime(acousticSensors->getInterpolationTime());

    connect(m_controllPanel, &ControllPanel::sigPositionSensorCountChanged,
            positionSensors.data(), &PositionSensors::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigAcousticSensorCountChanged,
            acousticSensors.data(), &AcousticSensors::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigModelCountChanged,
            trueModel.data(), &TrueModel::setScatterArraySize);

    connect(m_controllPanel, &ControllPanel::sigModelLenghtChanged,
            antennaModel.data(), &AntennaModel::setLenght);

    connect(m_controllPanel, &ControllPanel::sigPositionSensorEndChanged,
            positionSensors.data(), &PositionSensors::handleSetSensorEnd);

    connect(m_controllPanel, &ControllPanel::sigNoiseChanged,
            positionSensors.data(), &PositionSensors::handleSetNeedNoise);

    connect(m_controllPanel, &ControllPanel::sigInterpolationTypeChanged,
            acousticSensors.data(), &AcousticSensors::setInterpolationType);

    connect(m_controllPanel, &ControllPanel::sigMaxNoiseChanged,
            antennaModel.data(), &AntennaModel::handleMaxNoiseChanged);

    connect(m_controllPanel, &ControllPanel::sigEmulationButtonClicked,
            antennaModel.data(), &AntennaModel::handleStartEmulate);

    connect(m_controllPanel, &ControllPanel::sigIntervalModelChanged,
            antennaModel.data(), &AntennaModel::setInterval);

    connect(m_controllPanel, &ControllPanel::sigStepModelChanged,
            antennaModel.data(), &AntennaModel::setStep);

    connect(m_controllPanel, &ControllPanel::sigModelChanged,
            antennaModel.data(), &AntennaModel::handleCoefChanged);

    connect(m_controllPanel, &ControllPanel::sigModelSizeChanged,
            m_graph, &ScatterGraph::handleSetTrueModelSize);

    connect(m_controllPanel, &ControllPanel::sigModelColorChanged,
            m_graph, &ScatterGraph::handleSetTrueModelColor);

    connect(m_controllPanel, &ControllPanel::sigPositionSensorSizeChanged,
            m_graph, &ScatterGraph::handleSetPositionSensorSize);

    connect(m_controllPanel, &ControllPanel::sigAcousticSensorSizeChanged,
            m_graph, &ScatterGraph::handleSetAcousticSensorSize);

    connect(m_controllPanel, &ControllPanel::sigPositionSensorColorChanged,
            m_graph, &ScatterGraph::handleSetPositionSensorColor);

    connect(m_controllPanel, &ControllPanel::sigAcousticSensorColorChanged,
            m_graph, &ScatterGraph::handleSetAcousticSensorColor);

    connect(m_controllPanel, &ControllPanel::sigModelVisibilityChanged,
            m_graph, &ScatterGraph::handleSetTrueModelVisibility);

    connect(m_controllPanel, &ControllPanel::sigPositionSensorVisibilityChanged,
            m_graph, &ScatterGraph::handleSetPositionSensorVisibility);

    connect(m_controllPanel, &ControllPanel::sigAcousticSensorVisibilityChanged,
            m_graph, &ScatterGraph::handleSetAcousticSensorVisibility);

    connect(m_graph, &ScatterGraph::sigInterpolationTimeUpdate,
            m_controllPanel, &ControllPanel::handleUpdateInterpolationTime);

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

