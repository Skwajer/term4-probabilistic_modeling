#include "stepfigure.hpp"
#include <cmath>
#include <algorithm>

StepFigure::StepFigure(double M, double h, double tau, int n)
    : m_M(M)
    , m_h(h)
    , m_tau(tau)
    , m_n(n)
    , m_stepCount(static_cast<int>(M / h))
    , m_rng(std::random_device{}())
{}

void StepFigure::setDistribution(const QString& type, const QVariantMap& params)
{
    m_distributionType = type;
    m_distributionParams = params;
}

int64_t StepFigure::generateXi()
{
    if (m_distributionType == "uniform") {
        // Генерируем xi от 0 до m_n
        std::uniform_int_distribution<int64_t> dist(0, m_n);
        return dist(m_rng);
    }
    else if (m_distributionType == "binomial") {
        int trials = m_distributionParams.value("trials", 10).toInt();
        double p = m_distributionParams.value("p", 0.5).toDouble();
        std::binomial_distribution<int> binom(trials, p);
        return binom(m_rng) % (m_n + 1);
    }
    else if (m_distributionType == "finite_geometric") {
        double p = m_distributionParams.value("p", 0.5).toDouble();

        std::vector<double> weights(m_n + 1);
        double sum = 0.0;

        for (int i = 0; i <= m_n; i++) {
            weights[i] = std::pow(1.0 - p, i) * p;
            sum += weights[i];
        }

        for (auto& w : weights) w /= sum;

        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        return dist(m_rng);
    }
    else if (m_distributionType == "discrete_triangular") {
        int tn = m_distributionParams.value("tri_n", 10).toInt();

        std::vector<double> weights(m_n + 1);

        for (int i = 0; i <= m_n; i++) {
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

void StepFigure::generate()
{
    m_values.clear();

    for (int i = 0; i < m_stepCount; i++) {
        int64_t xi = generateXi();
        m_values.push_back(xi * m_tau);
    }
}

bool StepFigure::isStrictlyIncreasing() const
{
    if (m_values.size() < 2) {
        return true;
    }

    for (size_t i = 1; i < m_values.size(); i++) {
        if (m_values[i] <= m_values[i-1]) {
            qDebug() << "Not increasing at step" << i
                     << m_values[i] << "<=" << m_values[i-1];
            return false;
        }
    }

    qDebug() << "STRICTLY INCREASING! Values:" << m_values;
    return true;
}

void StepFigure::reset()
{
    m_values.clear();
}
