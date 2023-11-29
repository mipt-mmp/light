#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "physconstants.hpp"
#include "physicsthread.hpp"
#include <QDebug>
#include <QTimer>

const constexpr phys::Length XSize = 1e-2_m;
const constexpr phys::Length YSize = 1e-2_m;
const constexpr phys::Length ZDisplayerCoord = -1_m;


const constexpr phys::EFieldVal LightPower = phys::EFieldVal{1e-3};
const constexpr phys::Length WaveLength = 400e-9_m;

const constexpr phys::Length HolesDelta = (XSize + YSize) / phys::NoUnit{10.0};

const constexpr phys::Position LightPos = {XSize / phys::NoUnit{2.0}, YSize / phys::NoUnit{2.0}, ZDisplayerCoord + ZDisplayerCoord};
const constexpr phys::Position Hole1Pos = {LightPos.X(), LightPos.Y() + HolesDelta, ZDisplayerCoord};
const constexpr phys::Position Hole2Pos = {LightPos.X(), LightPos.Y() - HolesDelta, ZDisplayerCoord};


// const constexpr phys::Position DisplayerPos = {phys::Length{0.0}, phys::Length{0.0}, ZDisplayerCoord + ZDisplayerCoord};


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_physThread(new PhysicsThread(this)) {

    m_physThread->cont();

    auto* sources = new phys::PointLights();
    sources->addSource(std::make_pair(phys::EWave{LightPower}, LightPos));
    m_surfaces.addSurface(sources);

    m_barrier = new phys::PointsBarrier();
    m_barrier->addHole(Hole1Pos);
    m_barrier->addHole(Hole2Pos);
    m_surfaces.addSurface(m_barrier);

    ui->setupUi(this);

    m_surfaces.addSurface(ui->displayer->getSurface());

    m_surfaces.update();

    //    connect(ui->distSlider, SIGNAL(valueChanged(int)), this, SLOT(setDistance(int)));
    connect(ui->upd, SIGNAL(clicked()), this, SLOT(physRecalc()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

void MainWindow::toggleSimulation(bool run) {
    if (run) {
        m_physThread->cont();
        m_elapsed.start();
    }
    else
        m_physThread->stop();
}

void MainWindow::setSimulationSpeed(int x) {
    m_physThread->setPeriod(x);
}

void MainWindow::updateMetrics() {

}

void MainWindow::physRecalc()
{
    ui->displayer->resetColors();

    m_surfaces.setLight(phys::consts::green);
    ui->displayer->setCurrentColor(Qt::green);
    m_surfaces.update();

    m_surfaces.setLight(phys::consts::red);
    ui->displayer->setCurrentColor(Qt::red);
    m_surfaces.update();

    m_surfaces.setLight(phys::consts::blue);
    ui->displayer->setCurrentColor(Qt::blue);
    m_surfaces.update();
    
    ui->displayer->repaint();
}

void MainWindow::setDistance(int x){}
