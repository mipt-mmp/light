#ifndef MULTISLIDER_HPP
#define MULTISLIDER_HPP

#include "qstyle.h"
#include <QSlider>

class MultiSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MultiSlider(QWidget *parent = nullptr);
    ~MultiSlider() override;

    int addSlider();

    int addSlider(int value);

    void setSliderValue(int n, int value);

signals:
    void valueChangedNth(int n, int pos);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* ) override;

    void mouseMoveEvent(QMouseEvent* ) override;

    void keyPressEvent(QKeyEvent* ) override;
private:

    int pick(QPoint const& pt) { return orientation() == Qt::Horizontal ? pt.x() : pt.y(); }

    QVector<int> m_sliders;
    int m_activeSlider = -1;

    QStyle::SubControl m_pressedControl = QStyle::SC_None;
    int pixelPosToRangeValue(int pos);
    int m_clickOffset;

private slots:
    void sliderChangedValue(int);
};

#endif // MULTISLIDER_HPP
