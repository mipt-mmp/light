#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "chamber.hpp"
#include <QVector>
#include <QWidget>

class QSpinBox;
class QPushButton;

class ExperimentDisplayer : public QWidget {
    Q_OBJECT
public:
    explicit ExperimentDisplayer(phys::Chamber&& chamber, QWidget* parent = nullptr);
    ~ExperimentDisplayer() override;

    void setScale(phys::LengthVal scale) {
        m_scale = scale;
    }

private:
    phys::Chamber m_surfaces;
    phys::LengthVal m_scale;
    QTimer* m_timer;

    void resizeEvent(QResizeEvent* event) override;

    void rescale();

    QColor getColor(const phys::EWave&) const;
public slots:
    void paintEvent(QPaintEvent* event) override;
};

#endif // UNIVERSEDISPLAYER_HPP
