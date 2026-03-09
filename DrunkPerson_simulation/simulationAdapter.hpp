#ifndef SIMULATIONADAPTER_HPP
#define SIMULATIONADAPTER_HPP
#include <QObject>
#include <QVariantList>
#include <QVector>
#include <QTimer>
#include <QFile>

#include "simulation.hpp"

class simulationAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double q READ getq WRITE setq NOTIFY qChanged)
    Q_PROPERTY(double p READ getp WRITE setp NOTIFY pChanged)
    Q_PROPERTY(int B READ getB WRITE setB NOTIFY BChanged)
    Q_PROPERTY(int current_pos READ getCurrent_pos NOTIFY current_posChanged)
    Q_PROPERTY(int step_duration READ getStep_duration WRITE setStep_duration NOTIFY step_durationChanged)
    Q_PROPERTY(size_t max_steps_forward READ getMax_steps_forward WRITE setMax_steps_forward NOTIFY max_steps_forwardChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(size_t timeleft READ getTimeleft NOTIFY timeleftChanged)

    Q_PROPERTY(QVariantList fallProbabilities READ getFallProbabilities NOTIFY statsChanged)
    Q_PROPERTY(QVariantList pubProbabilities READ getPubProbabilities NOTIFY statsChanged)
    Q_PROPERTY(QVariantList bValues READ getBValues NOTIFY statsChanged)


public:
    void step();

    bool running() {return m_running;}

    explicit simulationAdapter(QObject *parent = nullptr);
    double getq() const { return m_simulation.get_q();}
    double getp() const { return m_simulation.get_p();}
    void setq(double new_q)
    {m_simulation.set_q(new_q); m_simulation.set_p(1 - new_q); emit qChanged(); emit pChanged();}
    void setp(double new_p)
    {m_simulation.set_p(new_p); m_simulation.set_q(1 - new_p); emit pChanged(); emit qChanged();}

    Q_INVOKABLE int getB() const { return m_simulation.get_B();}
    Q_INVOKABLE void setB(int newB) {
        m_simulation.set_B(newB);
        emit BChanged();
    }

    size_t getCurrent_pos() {return m_simulation.get_current_pos();}

    int getStep_duration() {return m_simulation.get_step_duration();}
    void setStep_duration(int new_step_dur) {m_simulation.set_step_duration(new_step_dur); emit step_durationChanged();}

    size_t getMax_steps_forward() {return m_simulation.get_max_steps_forward();}
    void setMax_steps_forward(size_t max_steps) {m_simulation.set_step_duration(max_steps); emit max_steps_forwardChanged();}

    size_t getTimeleft() {return m_timeleft;}

    Q_INVOKABLE void runStatistics(int numTrials = 1000);
    QVariantList getFallProbabilities() const {
        QVariantList list;
        for (double val : m_fallProbabilities) {
            list.append(val);
        }
        return list;
    }

    QVariantList getPubProbabilities() const {
        QVariantList list;
        for (double val : m_pubProbabilities) {
            list.append(val);
        }
        return list;
    }

    QVariantList getBValues() const {
        QVariantList list;
        for (int val : m_bValues) {
            list.append(val);
        }
        return list;
    }

signals:
    void qChanged();
    void pChanged();
    void BChanged();
    void current_posChanged();
    void step_durationChanged();
    void max_steps_forwardChanged();
    void runningChanged();
    void timeleftChanged();
    void statsChanged();


public slots:
    Q_INVOKABLE void runSimulation();
    void pauseSimulation();
    void resetSimulation();
    void onTimerTimeout();

private:
    Simulation m_simulation;
    bool m_running = false;
    QTimer m_timer;
    QTimer m_uiTimer;
    size_t m_timeleft = 0;

    QVector<double> m_fallProbabilities;
    QVector<double> m_pubProbabilities;
    QVector<int> m_bValues;
};
#endif // SIMULATIONADAPTER_HPP
