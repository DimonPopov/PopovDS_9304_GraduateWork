#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <Q3DScatter>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <QSlider>



//using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DScatter *surface);
    ~SurfaceGraph();

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
    Q3DScatter *m_graph;
    QScatterDataProxy *m_dataProxy;
    QScatter3DSeries *m_dataSeries;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void initTestData();
};

#endif // SURFACEGRAPH_H
