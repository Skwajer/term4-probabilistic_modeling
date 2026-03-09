#include "simulationAdapter.hpp"
#include <QThread>
#include <QDebug>

simulationAdapter::simulationAdapter(QObject *parent)
    : QObject(parent)
    , m_simulation(4, 400, 20)  // step_duration=500ms, max_steps=
    , m_running(false)
{
    emit step_durationChanged();
    emit max_steps_forwardChanged();
    emit BChanged();
    m_uiTimer.setInterval(500);
    connect(&m_uiTimer, &QTimer::timeout, this, &simulationAdapter::onTimerTimeout);
    m_timer.setTimerType(Qt::PreciseTimer);
    connect(&m_timer, &QTimer::timeout, this, &simulationAdapter::step);
}


void simulationAdapter::runSimulation()
{
    if (m_running) return;
    printf("%d\n", m_simulation.get_current_pos());

    m_simulation.run();
    if (m_simulation.get_current_pos() == 0)
    {
        resetSimulation();
    }

    m_running = true;
    m_uiTimer.start();
    m_timer.start(m_simulation.get_step_duration());
    emit runningChanged();
}

void simulationAdapter::pauseSimulation()
{
    if (!m_running) return;

    m_running = false;
    m_uiTimer.stop();
    m_timer.stop();
    emit runningChanged();
}

void simulationAdapter::resetSimulation()
{
    pauseSimulation();
    double old_q = m_simulation.get_q();
    double old_p = m_simulation.get_p();
    m_simulation = Simulation(
        m_simulation.get_B(),
        m_simulation.get_step_duration(),
        m_simulation.get_max_steps_forward()
        );

    m_simulation.set_q(old_q);
    m_simulation.set_p(old_p);
    emit qChanged();
    emit pChanged();
    emit current_posChanged();
}

void simulationAdapter::onTimerTimeout()
{
    m_timeleft++;
    timeleftChanged();
}

void simulationAdapter::step()
{
    if (!m_running) return;

    size_t currentPos = m_simulation.get_current_pos();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) < m_simulation.get_q()) {
        m_simulation.set_current_pos(currentPos - 1);
    } else {
        m_simulation.set_current_pos(currentPos + 1);
    }

    emit current_posChanged();

    size_t newPos = m_simulation.get_current_pos();
    if (newPos == 0 || newPos >= m_simulation.get_max_steps_forward()) {
        pauseSimulation();
    }
}

void simulationAdapter::runStatistics(int numTrials)
{
    qDebug() << "\n=== runStatistics START ===";
    Simulation sim_for_stats(4, 2000, 20);

    m_fallProbabilities.clear();
    m_pubProbabilities.clear();
    m_bValues.clear();

    int maxB = sim_for_stats.get_max_steps_forward();
    qDebug() << "maxB =" << maxB;

    for (int b = 1; b <= maxB; b++) {
        qDebug() << "\n--- Processing B =" << b << "---";
        sim_for_stats.set_B(b);

        int fallCount = 0;
        int pubCount = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            if (trial % 100 == 0) {
                qDebug() << "  trial" << trial << "/" << numTrials;
            }

            sim_for_stats.runUntilFinished();

            if (sim_for_stats.didFall()) {
                fallCount++;
            } else if (sim_for_stats.didReachPub()) {
                pubCount++;
            }
        }

        double fallProb = static_cast<double>(fallCount) / numTrials;
        double pubProb = static_cast<double>(pubCount) / numTrials;

        qDebug() << "  fallCount =" << fallCount << "prob =" << fallProb;
        qDebug() << "  pubCount  =" << pubCount << "prob =" << pubProb;

        m_fallProbabilities.append(fallProb);
        m_pubProbabilities.append(pubProb);
        m_bValues.append(b);
    }

    qDebug() << "\n=== runStatistics FINISH ===";
    qDebug() << "m_fallProbabilities size =" << m_fallProbabilities.size();
    qDebug() << "m_pubProbabilities size  =" << m_pubProbabilities.size();
    qDebug() << "m_bValues size           =" << m_bValues.size();

    emit statsChanged();
}
