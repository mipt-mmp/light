#ifndef PHYSICSTHREAD_HPP
#define PHYSICSTHREAD_HPP

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>

class PhysicsThread : public QThread {
    Q_OBJECT
public:
    PhysicsThread(QObject* parent = nullptr);
    ~PhysicsThread() override;

public slots:

    void stop() {
        m_stopped = true;
        emit toggled(false);
    }

    void cont() {
        m_mutex.lock();
        if (m_stopped) {
            m_stopped = false;
            m_allow_run.wakeAll();
            emit toggled(true);
        }
        m_mutex.unlock();
    }

    void toggle() {
        m_mutex.lock();
        m_stopped ^= 1;
        if (m_stopped) {
            m_mutex.unlock();
            emit toggled(false);
            return;
        }

        m_allow_run.wakeAll();
        emit toggled(true);
        m_mutex.unlock();
    }

public:
    [[nodiscard]] int getPeriod() const;
    [[nodiscard]] bool getStopped();

signals:
    void toggled(bool);

public slots:
    void setPeriod(int newPeriod);

protected:
    void run() override;

private:
    QMutex m_mutex;
    QWaitCondition m_allow_run;
    QAtomicInt m_stopped = true;
    QAtomicInt m_period = 32;
};

#endif // PHYSICSTHREAD_HPP
