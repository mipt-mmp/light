#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "physconstants.hpp"
#include "physicsthread.hpp"
#include <QDebug>
#include <QTimer>

const constexpr phys::Length XSize = 1e-2_m;
const constexpr phys::Length YSize = 1e-2_m;
const constexpr phys::Length ZDisplayerCoord = 2_m;


const constexpr phys::EFieldVal LightPower = phys::EFieldVal{1e-2};
const constexpr phys::Length WaveLength = 400e-9_m;

const constexpr phys::Length HolesDelta = (XSize + YSize) / phys::NoUnit{40.0};

const constexpr phys::Position LightPos = {XSize / phys::NoUnit{2.0}, YSize / phys::NoUnit{2.0}};
const constexpr phys::Position Hole1Pos = {LightPos.X(), LightPos.Y() + HolesDelta, ZDisplayerCoord};
const constexpr phys::Position Hole2Pos = {LightPos.X(), LightPos.Y() - HolesDelta, ZDisplayerCoord};


const constexpr phys::Position DisplayerPos = {phys::Length{0.0}, phys::Length{0.0}, ZDisplayerCoord + ZDisplayerCoord};


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_physThread(new PhysicsThread(this)) {


    m_physThread->cont();

    auto* sources = new phys::Lights();
    sources->AddSource(std::make_pair(phys::EWave{LightPower, WaveLength, phys::NoUnit{0.0}}, LightPos));
    m_surfaces.AddSurface(sources);

    m_barrier = new phys::Barrier();
    m_barrier->AddHole(Hole1Pos);
    m_barrier->AddHole(Hole2Pos);
    m_surfaces.AddSurface(m_barrier);

    m_displayer = new phys::Displayer(DisplayerPos, {XSize, YSize});
    m_surfaces.AddSurface(m_displayer);

    m_cd = new ExperimentDisplayer(std::move(m_surfaces), this);
    m_cd->setGeometry(rect());
    m_cd->setScale(XSize);

    ui->setupUi(this);
    connect(ui->distSlider, SIGNAL(valueChanged(int)), this, SLOT(setDistance(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    // m_cd->setGeometry(rect());
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

void MainWindow::setDistance(int x)
{
    // phys::Length delta = HolesDelta * phys::num_t{x / 100.};
    // phys::Position hole1Pos = {LightPos.X(), LightPos.Y() + delta, ZDisplayerCoord};
    // phys::Position hole2Pos = {LightPos.X(), LightPos.Y() - delta, ZDisplayerCoord};
    // m_barrier->SetHolePos(0, hole1Pos);
    // m_barrier->SetHolePos(1, hole2Pos);
    m_displayer->setZPos(ZDisplayerCoord + ZDisplayerCoord * phys::num_t{x / 1000.});
    m_cd->update();
}
