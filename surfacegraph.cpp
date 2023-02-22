#include "surfacegraph.h"

#include <QValue3DAxis>
#include <Q3DTheme>
#include <qmath.h>



SurfaceGraph::SurfaceGraph(Q3DScatter *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_dataProxy = new QScatterDataProxy();
    m_dataSeries = new QScatter3DSeries(m_dataProxy);

//    m_dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
//    m_dataSeries->setFlatShadingEnabled(true);

    initTestData();

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(-1.0f, 2.0f);
    m_graph->axisY()->setRange(-1.0f, 2.0f);
    m_graph->axisZ()->setRange(-1.0f, 2.0f);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    m_graph->addSeries(m_dataSeries);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::initTestData()
{
    QScatterDataArray *data = new QScatterDataArray;
    *data << QVector3D(0.5f, 0.5f, 0.5f) << QVector3D(-0.3f, -0.5f, -0.4f) << QVector3D(0.0f, -0.3f, 0.2f);

    m_dataProxy->resetArray(data);
}

//void SurfaceGraph::fillSqrtSinProxy()
//{
//    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
//    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

//    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
//    dataArray->reserve(sampleCountZ);

//    for (int i = 0 ; i < sampleCountZ; i++)
//    {
//        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
//        // Keep values within range bounds, since just adding step can cause minor drift due
//        // to the rounding errors.
//        float z = qMin(sampleMax, (i * stepZ + sampleMin));
//        int index = 0;
//        for (int j = 0; j < sampleCountX; j++)
//        {
//            float x = qMin(sampleMax, (j * stepX + sampleMin));
//            float R = qSqrt(z * z + x * x) + 0.01f;
//            float y = (qSin(R) / R + 0.24f) * 1.61f;
//            (*newRow)[index++].setPosition(QVector3D(x, y, z));
//        }
//        *dataArray << newRow;
//    }


//    m_dataProxy->resetArray(dataArray);
//}

//void SurfaceGraph::enableSqrtSinModel(bool enable)
//{
//    if (enable)
//    {
//        m_dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
//        m_dataSeries->setFlatShadingEnabled(true);

//        m_graph->axisX()->setLabelFormat("%.2f");
//        m_graph->axisZ()->setLabelFormat("%.2f");
//        m_graph->axisX()->setRange(-1.0f, 2.0f);
//        m_graph->axisY()->setRange(-1.0f, 2.0f);
//        m_graph->axisZ()->setRange(-1.0f, 2.0f);
//        m_graph->axisX()->setLabelAutoRotation(30);
//        m_graph->axisY()->setLabelAutoRotation(90);
//        m_graph->axisZ()->setLabelAutoRotation(30);

//        m_graph->axisX()->setTitle(QStringLiteral("X"));
//        m_graph->axisY()->setTitle(QStringLiteral("Y"));
//        m_graph->axisZ()->setTitle(QStringLiteral("Z"));

//        m_graph->axisX()->setTitleVisible(true);
//        m_graph->axisY()->setTitleVisible(true);
//        m_graph->axisZ()->setTitleVisible(true);

//        m_graph->addSeries(m_dataSeries);
//    }
//}

void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


