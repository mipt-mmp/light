#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "surface.hpp"
#include <QVector>
#include <QWidget>

class QSpinBox;
class QPushButton;

class ScreenDisplayer final : public QWidget, public phys::ContigSurface {
    Q_OBJECT
public:
    explicit ScreenDisplayer(QWidget* parent = nullptr);
    ~ScreenDisplayer() override;

    phys::Surface* getSurface() { return this; }

    void setCurrentColor(const QColor& newCurrentColor);

private:
    double m_brightness = 1.0;
    std::vector<QColor> m_colors;
    
    QColor m_currentColor = Qt::red;

    void resizeEvent(QResizeEvent* event) override;

    void update(const std::vector<phys::LightSource>& srcs) override;

    phys::Brightness m_maxBrightness;

    void recolor();
public slots:
    void 
    resetColors() {
        m_colors.assign(height()*width(), Qt::black);
    }

    void paintEvent(QPaintEvent* event) override;

    void setBrightness(int);
};

#endif // UNIVERSEDISPLAYER_HPP
