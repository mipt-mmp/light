#include "experimentdisplayer.hpp"

#include <QBrush>
#include <QDateTimeEdit>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <iostream>

#include "physconstants.hpp"

ExperimentDisplayer::ExperimentDisplayer(phys::Chamber&& metrics,
                                         QWidget* parent)
    : QWidget(parent), m_surfaces(metrics) {
  rescale();

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);
}

ExperimentDisplayer::~ExperimentDisplayer() {}

void ExperimentDisplayer::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  rescale();
}

void ExperimentDisplayer::rescale() {}

void ExperimentDisplayer::paintEvent(QPaintEvent* /*event*/) {
  QPainter painter(this);
  QPen pen;
  pen.setWidth(3);
  painter.setPen(pen);
  phys::num_t pixscale{std::min(rect().width(), rect().height())};

  // painter.drawRoundedRect(
  //     0, 0, static_cast<int>(pixscale * *(m_chamberMetrics.chamberCorner.X()
  //     / m_scale)), static_cast<int>(pixscale *
  //     *(m_chamberMetrics.chamberCorner.Y() / m_scale)), 3, 3);

  painter.setPen(pen);
  QBrush brush(Qt::SolidPattern);
  painter.setBrush(brush);

  auto metrics = m_surfaces.GetMetrics();
  auto sources = metrics.sources;
  std::cout << std::abs(*metrics.diff) << std::endl;

  for (auto& source : sources) {
    QColor color = getColor(source.first);

    brush.setColor(color);
    pen.setColor(color);
    painter.setPen(pen);
    painter.setBrush(brush);

    QPoint pt{static_cast<int>(pixscale * *((source.second.X()) / m_scale)),
              static_cast<int>(pixscale * *((source.second.Y()) / m_scale))};

    painter.drawPoint(pt);
  }
}

QColor ExperimentDisplayer::getColor(const phys::EWave& wave) const {
  int v = std::min(100, static_cast<int>(*wave.getIntensity() * 0.2e6));
  return QColor::fromHsv(0, 0, v);
}
