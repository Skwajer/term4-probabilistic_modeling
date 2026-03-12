#include "simulation.hpp"
#include <cmath>
#include <QDebug>

Simulation::Simulation(double rho, int n)
    : m_rho(rho)
    , m_n(n)
    , m_rng(std::random_device{}())
    , m_indexDist(0, 0)
    , m_returnedToOrigin(false)
    , m_stepsToReturn(-1)
    , m_currentStep(0)
{}

void Simulation::setXiValues(const std::vector<int64_t>& values)
{
    m_xiValues = values;
    if (!values.empty()) {
        m_indexDist = std::uniform_int_distribution<size_t>(0, values.size() - 1);
    }
}

int64_t Simulation::generateXi()
{
    if (!m_xiValues.empty()) {
        size_t index = m_indexDist(m_rng);
        return m_xiValues[index];
    }

    if (m_distributionType == "uniform") {
        std::uniform_int_distribution<int64_t> dist(0, m_n - 1);
        return dist(m_rng);
    }
    else if (m_distributionType == "binomial") {
        int trials = m_distributionParams.value("trials", 10).toInt();
        double p = m_distributionParams.value("p", 0.5).toDouble();
        std::binomial_distribution<int> binom(trials, p);
        return binom(m_rng) % m_n;
    }
    else if (m_distributionType == "finite_geometric") {
        double p = m_distributionParams.value("p", 0.5).toDouble();

        std::vector<double> weights(m_n);
        double sum = 0.0;

        for (int i = 0; i < m_n; i++) {
            weights[i] = std::pow(1.0 - p, i) * p;
            sum += weights[i];
        }

        for (auto& w : weights) w /= sum;

        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        return dist(m_rng);
    }
    else if (m_distributionType == "discrete_triangular") {
        int tn = m_distributionParams.value("tri_n", 10).toInt();

        std::vector<double> weights(m_n);

        for (int i = 0; i < m_n; i++) {
            int k = i % (tn + 1);
            double w = (k <= tn / 2)
                           ? static_cast<double>(k + 1)
                           : static_cast<double>(tn - k + 1);
            weights[i] = std::max(w, 1.0);
        }

        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        return dist(m_rng);
    }

    return 0;
}

void Simulation::setDistribution(const QString& type, const QVariantMap& params)
{
    m_distributionType = type;
    m_distributionParams = params;
}

void Simulation::run(int max_steps)
{
    m_trajectory.clear();
    m_trajectory.push_back(std::complex<double>(0, 0));
    m_returnedToOrigin = false;
    m_stepsToReturn = -1;

    for (m_currentStep = 1; m_currentStep <= max_steps; ++m_currentStep) {
        int64_t xi = generateXi();

        double angle = (2.0 * M_PI / m_n) * xi;

        std::complex<double> step = std::polar(m_rho, angle);
        std::complex<double> newPos = m_trajectory.back() + step;

        m_trajectory.push_back(newPos);

        if (std::abs(newPos) < 1e-7) {
            m_returnedToOrigin = true;
            m_stepsToReturn = m_currentStep;
            break;
        }
    }
}
void Simulation::reset()
{
    m_trajectory.clear();
    m_returnedToOrigin = false;
    m_stepsToReturn = -1;
    m_currentStep = 0;
}
