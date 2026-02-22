#include "simulation.hpp"
#include <chrono>
Simulation::Simulation()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

Person *Simulation::getPerson(int id)
{
    if (id >=0 && id < people.size())
    {
        return people[id];
    }
    return nullptr;
}

void Simulation::step()
{
    std::vector<Person *> infectious;
    for (auto &person : people)
    {
        if (person->state == State::Infectious)
        {
            infectious.push_back(person);
        }
    }
    
    std::vector<Person *> recovered;
    for (auto &sick_person : infectious)
    {
        if (real_dist(rng) < p2)
        {
            sick_person->state = State::Recovered;
            recovered.push_back(sick_person);
        }
    }
    
    for (auto &sick : infectious)
    {
        if (sick->state != State::Infectious) continue;
        for (auto contact_id : sick->contacts)
        {
            auto contact = getPerson(contact_id);
            if (contact->state == State::Susceptible)
            {
                if (real_dist(rng) < p1)
                {
                    contact->state = State::Infectious;
                }
            }
        }
    }

    updateCounts();

    if (stepCallback)
    {
        // signal for simulationAdapter about step done
        stepCallback();
    }
}

void Simulation::infect_first_person()
{
    if (people.empty()) return;
    std::uniform_int_distribution<size_t> int_dist(0, people.size() - 1);
    size_t index = int_dist(rng);
    people[index]->state = State::Infectious;
}

void Simulation::updateCounts()
{
    m_susceptibleCount = 0;
    m_infectedCount = 0;
    m_recoveredCount = 0;

    for (auto person : people)
    {
        switch (person->state) {
        case State::Susceptible: m_susceptibleCount++; break;
        case State::Infectious: m_infectedCount++; break;
        case State::Recovered: m_recoveredCount++; break;
        }
    }
}

bool Simulation::loadGraph(int numPeople, const std::vector<std::pair<int, int>>& edges)
{
    people.clear();

    people.reserve(numPeople);
    for (int i = 0; i < numPeople; ++i)
    {
        printf("hello");
        Person *person = new Person(i);
        people.push_back(person);
    }

    for (const auto& edge : edges)
    {
        int it1 = edge.first;
        int it2 = edge.second;

        Person* p1 = people[it1];
        Person* p2 = people[it2];

        p1->contacts.push_back(it2);
        p2->contacts.push_back(it1);
    }
    updateCounts();
    return true;

}

void Simulation::run()
{
    infect_first_person();
}

void Simulation::reset()
{
    for (auto person : people)
    {
        person->state = State::Susceptible;
    }
    updateCounts();
}










