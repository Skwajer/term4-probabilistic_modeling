#include "simulation.hpp"
#include <cmath>

Simulation::Simulation(double step_size, double p_probability)
    : m_s(step_size), m_p(p_probability), m_rng(std::random_device{}())
{}

int Simulation::runUntilReturnToZero()
{
    double pos = 0.0;
    int steps = 0;

    while (true) {
        steps++;
        if (steps > 1000000000) return -1;

        pos += (m_real_dist(m_rng) < m_p) ? m_s : -m_s;

        if (std::abs(pos) < 1e-9) return steps;
    }
}

void Simulation::setP(double new_p) { m_p = new_p; }
double Simulation::getP() const { return m_p; }
double Simulation::getStepSize() const { return m_s; }