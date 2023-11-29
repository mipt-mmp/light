#include "ScreenDisplayer.hpp"

#include <QBrush>
#include <QDateTimeEdit>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <iostream>

#include "sizes.hpp"

ScreenDisplayer::ScreenDisplayer(QWidget* parent)
    : QWidget(parent), phys::ContigSurface({XBoxSize, XBoxSize})
{
    resetColors();
    setResolution(std::min(width(), height()));
}

ScreenDisplayer::~ScreenDisplayer() {}

void
ScreenDisplayer::setCurrentColor(const QColor& newCurrentColor)
{
    m_currentColor = newCurrentColor;
}

void
ScreenDisplayer::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    QPen pen;
    
    assert(m_colors.size() == width() * height());

    for(int i = 0; i < width(); ++i) {
        for(int j = 0; j < height(); ++j) {
            pen.setColor(m_colors[i*height() + j]);
            painter.setPen(pen);
            painter.drawPoint(i, j);
        }
    }
}

void 
ScreenDisplayer::resizeEvent(QResizeEvent* event){
  QWidget::resizeEvent(event);
  assert(height() == width());
  setResolution(width());
  resetColors();
}

void 
ScreenDisplayer::update(const std::vector<phys::LightSource>& srcs) {
    phys::ContigSurface::update(srcs);
    const auto& lights = getSrcs();
    if(lights.empty())
        return;

    std::cerr << width() << ' ' << height() << ' ' << lights.size() << '\n';
    if(lights.size() != width() * height()) {
        qDebug() << "lights.size() != width() * height()\n";
        return;
    }

    phys::Brightness maxBrightness;

    for(const auto& [wave,_] : lights) {
        maxBrightness = std::max(maxBrightness, wave.getIntensity());
    }

    for(int i = 0; i < width(); ++i) {
        for(int j = 0; j < height(); ++j) {
            double value = m_brightness * (lights[i*height() + j].first.getIntensity() / maxBrightness)->getVal();
            value = std::min(1., value);
            if(m_currentColor == Qt::red) {
                m_colors[i*height()+j].setRedF(static_cast<float>(value));
            } else if (m_currentColor == Qt::green) {
                m_colors[i*height()+j].setGreenF(static_cast<float>(value));
            } else if (m_currentColor == Qt::blue) {
                m_colors[i*height()+j].setBlueF(static_cast<float>(value));
            } else {
                qDebug() << "Wrong color\n";
                abort();
            }
        }
    }
}