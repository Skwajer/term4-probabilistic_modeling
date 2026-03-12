#include "simulation.hpp"
#include <cmath>
#include <QDebug>

Simulation::Simulation() : m_rng(std::random_device{}())
{

}

void Simulation::configure(double h, int n, int K, int l, const QVector<double>& s)
{
    m_h = h;
    m_n = n;
    m_K = K;
    m_l = l;
    m_s = s;
    reset();
}

void Simulation::setDistribution(const QString& type, const QVariantMap& params)
{
    m_distributionType = type;
    m_distributionParams = params;
}

double Simulation::generateRandomStep()
{
    if (m_s.empty()) return 0.0;

    if (m_distributionType == "uniform") {
        std::uniform_int_distribution<> dist(0, m_s.size() - 1);
        size_t random_increment = dist(m_rng);
        qDebug() << "random = " << random_increment;
        return m_s[random_increment];
    }
    else if (m_distributionType == "binomial") {

        double p = m_distributionParams.value("p", 0.5).toDouble();

        std::vector<double> positive, negative;
        for (double val : m_s) {
            if (val >= 0) positive.push_back(val);
            else negative.push_back(val);
        }

        if (positive.empty() || negative.empty()) {
            std::uniform_int_distribution<> dist(0, m_s.size() - 1);
            return m_s[dist(m_rng)];
        }

        std::binomial_distribution<> binom(1, p);
        if (binom(m_rng)) {
            std::uniform_int_distribution<> dist(0, positive.size() - 1);
            return positive[dist(m_rng)];
        } else {
            std::uniform_int_distribution<> dist(0, negative.size() - 1);
            return negative[dist(m_rng)];
        }
    }
    else if (m_distributionType == "geometric") {
        double p = m_distributionParams.value("p", 0.5).toDouble();
        std::geometric_distribution<> geom(p);
        int index = geom(m_rng) % m_s.size();
        return m_s[index];
    }
    else if (m_distributionType == "triangular") {
        std::uniform_int_distribution<> dist(0, m_s.size() - 1);
        return m_s[dist(m_rng)];
    }

    return 0.0;
}
void Simulation::run()
{
    m_trajectory.clear();
    m_currentX = 0.0;
    m_currentY = generateRandomStep();
    m_intersectionCount = 0;
    m_trajectory.push_back(m_currentY);

    bool wasAbove = (m_currentY >= 0);

    for (int i = 0; i < m_n - 1; i++)
    {
        printf("hello i = %d \n", i);
        m_currentX += m_h;

        double s = generateRandomStep();

        m_currentY += s;

        m_trajectory.push_back(m_currentY);

        bool isAbove = (m_currentY >= 0);
        if (wasAbove != isAbove) {
            m_intersectionCount++;
            wasAbove = isAbove;
        }
    }

    qDebug() << "Simulation finished. Intersections:" << m_intersectionCount;
}

void Simulation::step()
{
    bool was_above = m_currentY >= 0;
    m_currentX += m_h;
    m_currentY += generateRandomStep();
    m_trajectory.push_back(m_currentY);
    bool is_above = m_currentY >= 0;
    if (was_above != is_above)
    {
        m_intersectionCount++;
        was_above = is_above;

    }
    qDebug() << "Y =" << m_currentY;
}

void Simulation::reset()
{
    m_currentX = 0.0;
    m_currentY = 0.0;
    m_trajectory.clear();
    m_intersectionCount = 0;
}
