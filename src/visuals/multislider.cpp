#include "multislider.hpp"
#include "qapplication.h"

#include <QPainter>
#include <QStylePainter>
#include <QStyleOption>
#include <QMouseEvent>

MultiSlider::MultiSlider(QWidget *parent) :
    QSlider(parent)
{}

MultiSlider::~MultiSlider()
{
    connect(this, SIGNAL(actionTriggered(int)), this, SLOT(sliderChangedValue(int)));
}

int
MultiSlider::addSlider()
{
    m_sliders.push_back(minimum());
    return m_sliders.size() - 1;
}

int MultiSlider::addSlider(int value)
{
    m_sliders.push_back(value);
    return m_sliders.size() - 1;
}

void
MultiSlider::setSliderValue(int n, int value) {
    m_sliders[n] = value;
    update();
}

void MultiSlider::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QStylePainter sp(this);

    QStyleOptionSlider opt;
#if 1
    // Draw groove
    initStyleOption(&opt);
    opt.sliderValue = 0;
    opt.sliderPosition = 0;
    opt.subControls = QStyle::SC_SliderGroove;
    sp.drawComplexControl(QStyle::CC_Slider, opt);
#endif

    QStyle* style = this->style();
    for(int sliderPos : m_sliders) {

        this->initStyleOption(&opt);
        opt.subControls = QStyle::SC_SliderHandle; // | QStyle::SC_SliderGroove;
        if (this->tickPosition() != QSlider::NoTicks) opt.subControls |= QStyle::SC_SliderTickmarks;
        if (m_pressedControl) opt.activeSubControls = m_pressedControl;
//        else opt.activeSubControls = this->hover_control;
        opt.sliderPosition = sliderPos;
        opt.sliderValue    = sliderPos;
        style->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);
    }
}

void MultiSlider::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        event->accept();
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        m_activeSlider = -1;

        QStyle* style = QApplication::style();
        QStyle::SubControl hit;

        for(int i = 0; i < m_sliders.size(); ++i) {
            initStyleOption(&opt);
            opt.sliderPosition = m_sliders[i];
            opt.sliderValue = m_sliders[i];
            QAbstractSlider::setValue(m_sliders[i]);
            hit = style->hitTestComplexControl(QStyle::CC_Slider, &opt, event->pos(), this);
            if (hit == QStyle::SC_SliderHandle) {
                m_activeSlider = i;
                m_pressedControl = hit;
                triggerAction(this->SliderMove);
                setRepeatAction(this->SliderNoAction);
                setSliderDown(true);
                QRect sr = style->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
                m_clickOffset = pick(event->position().toPoint() - sr.topLeft());
                break;
            }
        }
    } else {
        event->ignore();
    }
    if(m_activeSlider >= 0) {
        QSlider::setValue(m_sliders[m_activeSlider]);
    }

    QSlider::mousePressEvent(event);
}

void MultiSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pressedControl != QStyle::SC_SliderHandle) {
        event->ignore();
        return;
    }

    event->accept();
    int new_pos = pixelPosToRangeValue(pick(event->position().toPoint()) - m_clickOffset);
    QStyleOptionSlider opt;
    this->initStyleOption(&opt);

    if (m_activeSlider >= 0) {
        if (new_pos >= maximum())
            new_pos = maximum();
        m_sliders[m_activeSlider] = new_pos;
    }

    update();
    emit valueChangedNth(m_activeSlider, new_pos);
}

void MultiSlider::keyPressEvent(QKeyEvent* ev)
{
    QSlider::keyPressEvent(ev);
}

int MultiSlider::pixelPosToRangeValue(int pos)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QStyle* style = QApplication::style();

    QRect gr = style->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    int slider_length, slider_min, slider_max;
    if (this->orientation() == Qt::Horizontal) {
        slider_length = sr.width();
        slider_min = gr.x();
        slider_max = gr.right() - slider_length + 1;
    } else {
        slider_length = sr.height();
        slider_min = gr.y();
        slider_max = gr.bottom() - slider_length + 1;
    }

    return style->sliderValueFromPosition(this->minimum(), this->maximum(), pos-slider_min, slider_max-slider_min, opt.upsideDown);
}

void MultiSlider::sliderChangedValue(int x)
{
    qDebug() << "New value: " << x << '\n';
    m_sliders[m_activeSlider] = x;
    update();
}

