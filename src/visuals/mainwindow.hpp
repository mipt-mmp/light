#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chamberdisplayer.hpp"
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
    
    QElapsedTimer m_elapsed;

    PhysicsThread* m_physThread;

    size_t m_currentAtom = 0;

private slots:
    void toggleSimulation(bool);
    void setSimulationSpeed(int);

    void updateMetrics();

    void setXLength(int);

    void openHole(bool);

    void setColoring(int);
};

#endif // MAINWINDOW_H
