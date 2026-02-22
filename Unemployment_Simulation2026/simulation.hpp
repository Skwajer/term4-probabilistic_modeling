#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <functional>
#include <random>
#include<vector>

enum State
{
    Susceptible,
    Infectious,
    Recovered
};

struct Person
{
    int id;
    std::vector<int> contacts;
    State state;

    Person(int init_id) : state(State::Susceptible), id(init_id)
    {}
};

class Simulation
{
public:
    Simulation();
    void run();
    void step();
    void reset();
    void setRandomSeed(unsigned seed) {rng.seed(seed);}

    bool loadGraph(int numPeople, const std::vector<std::pair<int, int>>& edges);

    int getInfectedCount() const { return m_infectedCount; }
    int getRecoveredCount() const { return m_recoveredCount; }
    int getSusceptibleCount() const { return m_susceptibleCount; }
    int getPersonCount() const { return people.size(); }
    void setP1(double init_p1) { p1 = init_p1; }
    void setP2(double init_p2) { p2 = init_p2; }
    double getP1() const { return p1; }
    double getP2() const { return p2; }
    Person *getPerson(int id);

    void setStepCallback(std::function<void()> callback)
    {
        stepCallback = callback;
    }

private:
    std::function<void()> stepCallback;
    void infect_first_person();
    std::vector<Person *> people;
    std::mt19937 rng;
    std::uniform_real_distribution<double> real_dist{0.0, 1.0};
    double p1 = 0.2;
    double p2 = 0.3;
    int m_infectedCount = 0;
    int m_recoveredCount = 0;
    int m_susceptibleCount = 0;

    void updateCounts();
};

#endif // SIMULATION_HPP

