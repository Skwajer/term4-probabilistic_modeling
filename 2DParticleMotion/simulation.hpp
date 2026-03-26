#pragma once
#include <random>

class Simulation
{
public:
    Simulation(double step_size, double p_probability);
    int runUntilReturnToZero();
    void setP(double new_p);
    double getP() const;
    double getStepSize() const;

private:
    double m_s;
    double m_p;
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_real_dist{0.0, 1.0};
};