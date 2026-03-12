#include "simulation.hpp"

Simulation::Simulation(int init_B, int step_duration, size_t max_steps)
{
    m_B = init_B;
    m_current_pos = m_B;
    m_step_duration = step_duration;
    m_max_steps_forward = max_steps;
}

void Simulation::run()
{
    while(m_current_pos != 0)
    {
        if (m_current_pos >= m_max_steps_forward)
        {
            break;
        }
        m_current_pos += (m_real_dist(m_rng) < m_q) ? -1 : 1;
    }
}

void Simulation::runUntilFinished()
{
    m_didFall = false;
    m_didReachPub = false;
    m_stepsTaken = 0;
    m_step_duration = 3000;

    m_current_pos = m_B;

    while (true) {
        m_stepsTaken++;

        if (m_real_dist(m_rng) < m_q) {
            m_current_pos--;
        } else {
            m_current_pos++;
        }

        if (m_current_pos == 0) {
            m_didFall = true;
            break;
        }

        if (m_current_pos >= m_max_steps_forward) {
            m_didReachPub = true;
            break;
        }

        if (m_stepsTaken > 10000) {
            break;
        }
    }
}

void Simulation::resetStats()
{
    m_didFall = false;
    m_didReachPub = false;
    m_stepsTaken = 0;
}
