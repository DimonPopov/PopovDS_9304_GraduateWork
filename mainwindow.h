#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ControllPanel;
class ScatterGraph;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ScatterGraph* m_graph;
    ControllPanel* m_controllPanel;
};
#endif // MAINWINDOW_H
