#pragma once
#include <random>
#include <vector>
#include <complex>
#include <QString>
#include <QVariantMap>

class Simulation
{
public:
    Simulation(double rho, int n);

    void setDistribution(const QString& type, const QVariantMap& params);
    void setXiValues(const std::vector<int64_t>& values);  // массив значений ξ
    void run(int max_steps = 1000000);
    void reset();

    // Геттеры
    const std::vector<std::complex<double>>& getTrajectory() const { return m_trajectory; }
    bool returnedToOrigin() const { return m_returnedToOrigin; }
    int getStepsToReturn() const { return m_stepsToReturn; }
    int getCurrentStep() const { return m_currentStep; }
    double get_p() {return m_rho;}
    std::vector<int64_t> get_xiValues() {return m_xiValues;}

private:
    int64_t generateXi();  // выбор из m_xiValues

    int m_n;
    double m_rho;
    std::mt19937_64 m_rng;
    std::uniform_int_distribution<size_t> m_indexDist;  // для выбора индекса

    QString m_distributionType;
    QVariantMap m_distributionParams;
    std::vector<int64_t> m_xiValues;  // массив возможных значений ξ

    std::vector<std::complex<double>> m_trajectory;
    bool m_returnedToOrigin;
    int m_stepsToReturn;
    int m_currentStep;
};
