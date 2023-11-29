#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ScreenDisplayer.hpp"
#include "chamber.hpp"
#include <QMainWindow>
#include <array>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}
class PhysicsThread;
class QTimer;
class QElapsedTimer;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MainWindow* ui;

    QTimer* m_timer;
    ScreenDisplayer* m_cd;
    
    QElapsedTimer m_elapsed;

    phys::Chamber m_surfaces;
    PhysicsThread* m_physThread;

    size_t m_currentAtom = 0;

    phys::PointsBarrier* m_barrier;
    phys::Displayer* m_displayer;
private slots:
    void toggleSimulation(bool);
    void setSimulationSpeed(int);

    void updateMetrics();

    void physRecalc();

    void setDistance(int);
};

#endif // MAINWINDOW_H
