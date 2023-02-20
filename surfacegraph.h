#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <Q3DSurface>
#include <QSurfaceDataProxy>
#include <QHeightMapSurfaceDataProxy>
#include <QSurface3DSeries>
#include <QSlider>

using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DSurface *surface);
    ~SurfaceGraph();
//    void enableSqrtSinModel(bool enable);

    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

public slots:
    void changeTheme(int theme);

private:
    Q3DSurface *m_graph;
    QSurfaceDataProxy *m_dataProxy;
    QSurface3DSeries *m_dataSeries;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void initTestData();
};

#endif // SURFACEGRAPH_H
