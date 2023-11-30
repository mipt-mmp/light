#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "physconstants.hpp"
#include "physicsthread.hpp"
#include "sizes.hpp"
#include <QDebug>
#include <QTimer>


// const constexpr phys::Position DisplayerPos = {phys::Length{0.0}, phys::Length{0.0}, ZDisplayerCoord + ZDisplayerCoord};


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_physThread(nullptr) {

//    m_physThread->cont();
    ui->setupUi(this);

//    presetYng();
   presetDifr();
    // presetLens();

    m_surfaces.addSurface(ui->displayer->getSurface());
    m_surfaces.update();

    connectControls();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if(m_tracking) {
        physRecalc();
    }
}

void MainWindow::presetYng()
{
    constexpr const phys::Length YngLightZ   = -2_m;
    constexpr const phys::Length YngBarrierZ = -1_m;

    ui->horizontalSlider->setMinimum(2 * (YngLightZ / ZScale)->getVal());

    m_lights = new phys::PointLights();
    m_lights->addSource(std::make_pair(phys::EWave{phys::EFieldVal{1.}}, phys::Position{XMid, XMid, YngLightZ}));
    m_surfaces.addSurface(m_lights);
    ui->horizontalSlider->addSlider((YngLightZ / ZScale)->getVal());

    constexpr const phys::Position Hole2Pos = {XMid, XMid + 1e-3_m, YngBarrierZ};
    constexpr const phys::Position Hole1Pos = {XMid, XMid - 1e-3_m, YngBarrierZ};

    auto* barrier = new phys::PointsBarrier();
    barrier->addHole(Hole1Pos);
    barrier->addHole(Hole2Pos);
    m_surfaces.addSurface(barrier);
    ui->horizontalSlider->addSlider((barrier->getZ() / ZScale)->getVal());
}


void MainWindow::presetDifr()
{
    m_tracking = false;

    constexpr const phys::Length DifrLightZ   = -2_m;
    constexpr const phys::Length DifrBarrierZ = -1_m;
    constexpr const phys::Position DifrLightPos{XMid, XMid, DifrLightZ};
    constexpr const phys::Position DifrBarrierPos{XBoxSize, XBoxSize, DifrBarrierZ};

    ui->horizontalSlider->setMinimum(2 * (DifrLightZ / ZScale)->getVal());

    m_lights = new phys::PointLights();
    m_lights->addSource(std::make_pair(phys::EWave{phys::EFieldVal{1.}}, DifrLightPos));
    m_surfaces.addSurface(m_lights);
    ui->horizontalSlider->addSlider((DifrLightZ / ZScale)->getVal());

    phys::Length redLen = phys::consts::c / phys::consts::red;

    auto transparent = [=](phys::Position pos) -> bool {
        auto lens = ((DifrLightPos - pos).Len() + (pos - phys::Position{XMid, XMid}).Len()) / redLen;
        double val = lens->getVal();
//        std::cerr << (val - static_cast<long>(val)) << "\n";
        return (val - static_cast<long>(val)) <= 0.5;
    };

    auto* barrier = new phys::ContigSurface(DifrBarrierPos, transparent);
    m_surfaces.addSurface(barrier);
    ui->horizontalSlider->addSlider((barrier->getZ() / ZScale)->getVal());
    barrier->setResolution(10);
    //    barrier->setResolution(50); // Sloooowly generates caleidoscope;
}

void MainWindow::presetLens()
{
    m_tracking = false;

    constexpr const phys::Length DifrLightZ   = -2_m;
    constexpr const phys::Length DifrBarrierZ = -1_m;
    constexpr const phys::Position DifrLightPos{XMid, XMid, DifrLightZ};
    constexpr const phys::Position DifrBarrierPos{XBoxSize, XBoxSize, DifrBarrierZ};
    constexpr const phys::Position DifrBarrierPos2{XBoxSize, XBoxSize, DifrBarrierZ + 10.__ * ZScale};

    ui->horizontalSlider->setMinimum(2 * (DifrLightZ / ZScale)->getVal());

    m_lights = new phys::PointLights();
    m_lights->addSource(std::make_pair(phys::EWave{phys::EFieldVal{1.}}, DifrLightPos));
    m_surfaces.addSurface(m_lights);
    ui->horizontalSlider->addSlider((DifrLightZ / ZScale)->getVal());

    phys::Length redLen = phys::consts::c / phys::consts::red;

    auto transform = [=](phys::Position pos) -> phys::Position {
        pos.Z() += phys::sqrt(XMid * XMid - (pos.X() - XMid) * (pos.X() - XMid) + (pos.Y() - XMid) * (pos.Y() - XMid));
        return pos;
    };

    auto* barrier1 = new phys::ContigSurface(DifrBarrierPos);
    m_surfaces.addSurface(barrier1);
    ui->horizontalSlider->addSlider((barrier1->getZ() / ZScale)->getVal());
    barrier1->setResolution(30);

    auto* barrier2 = new phys::ContigSurface(DifrBarrierPos2, transform);
    m_surfaces.addSurface(barrier2);
    ui->horizontalSlider->addSlider((barrier2->getZ() / ZScale)->getVal());
    barrier2->setResolution(30);

    m_surfaces.setRefractiveIndex(1, 1.43__);
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

void MainWindow::animation() {
    for(int i = ui->horizontalSlider->maximum(); i > ui->horizontalSlider->minimum() / 2; i -= 3) {
        ui->horizontalSlider->setSliderValue(1, i);
        setDistance(1, i);
        if(!m_tracking)
            physRecalc();
        update();
    }
    // setDistance(1, ui->horizontalSlider->va);  
}


void MainWindow::physRecalc()
{
    ui->displayer->resetColors();
    if(m_lights == nullptr) return;

    if(ui->RPower->value() != 0) {
        m_lights->setPower(phys::EFieldVal(ui->RPower->value()));
        m_surfaces.setLight(phys::consts::red);
        ui->displayer->setCurrentColor(Qt::red);
        m_surfaces.update();
    }

    if(ui->GPower->value() != 0) {
        m_lights->setPower(phys::EFieldVal(ui->GPower->value()));
        m_surfaces.setLight(phys::consts::green);
        ui->displayer->setCurrentColor(Qt::green);
        m_surfaces.update();
    }

    if(ui->BPower->value() != 0) {
        m_lights->setPower(phys::EFieldVal(ui->BPower->value()));
        m_surfaces.setLight(phys::consts::blue);
        ui->displayer->setCurrentColor(Qt::blue);
        m_surfaces.update();
    }
    
    ui->displayer->repaint();
}

void MainWindow::setDistance(int n, int x){

    m_surfaces.setZ(n, phys::num_t{x} * ZScale);
}

void MainWindow::connectControls()
{
    connect(ui->brightness, SIGNAL(valueChanged(int)), ui->displayer, SLOT(setBrightness(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChangedNth(int,int)), this, SLOT(setDistance(int,int)));

    if(m_tracking) {
        connect(ui->RPower, SIGNAL(valueChanged(int)), this, SLOT(physRecalc()));
        connect(ui->GPower, SIGNAL(valueChanged(int)), this, SLOT(physRecalc()));
        connect(ui->BPower, SIGNAL(valueChanged(int)), this, SLOT(physRecalc()));
        connect(ui->brightness, SIGNAL(valueChanged(int)), this, SLOT(physRecalc()));
        connect(ui->horizontalSlider, SIGNAL(valueChangedNth(int,int)), this, SLOT(physRecalc()));
    }
    connect(ui->upd, SIGNAL(clicked()), this, SLOT(physRecalc()));
    connect(ui->anime, SIGNAL(clicked()), this, SLOT(animation()));

}
