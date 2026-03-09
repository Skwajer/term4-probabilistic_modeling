#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include <random>
#include <chrono>
#include <unistd.h>
#include <thread>

class Simulation
{
public:
    Simulation(int init_B, int step_duration, size_t max_steps);
    void run();

    void set_q(double new_q) {m_q = new_q;}
    void set_p(double new_p) {m_p = new_p;}
    double get_q() const {return m_q;}
    double get_p() const {return m_p;}

    void set_B(double new_B) {m_B = new_B; m_current_pos = new_B;}
    int get_B() const {return m_B;}

    size_t get_current_pos() const {return m_current_pos;}
    void set_current_pos(size_t pos) { m_current_pos = pos;}

    int get_step_duration() const {return m_step_duration;}
    void set_step_duration(int new_duration) {m_step_duration = new_duration;}

    size_t get_max_steps_forward() const {return m_max_steps_forward;}
    void set_max_steps_forward(size_t new_steps_forward) {m_max_steps_forward = new_steps_forward;}

    void runUntilFinished();
    bool didFall() const { return m_didFall; }
    bool didReachPub() const { return m_didReachPub; }
    void resetStats();

private:
    int m_B = 3;
    double m_p = 0.4;
    double m_q = 0.6;
    size_t m_current_pos;
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_real_dist {0.0, 1.0};
    std::uniform_int_distribution<int> m_int_dist {0, 30};
    int m_step_duration = 300;
    size_t m_max_steps_forward = 20;

    bool m_didFall = false;
    bool m_didReachPub = false;
    int m_stepsTaken = 0;
};

#endif // SIMULATION_HPP
