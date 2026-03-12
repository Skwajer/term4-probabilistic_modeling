#ifndef STEPFIGURE_HPP
#define STEPFIGURE_HPP
#pragma once
#include <random>
#include <vector>
#include <QString>
#include <QVariantMap>

class StepFigure
{
public:
    StepFigure(double M, double h, double tau, int n);

    void setDistribution(const QString& type, const QVariantMap& params);
    void generate();
    void reset();

    // Геттеры
    const std::vector<double>& getValues() const { return m_values; }
    bool isStrictlyIncreasing() const;
    double getStepCount() const { return m_stepCount; }
    double getM() const { return m_M; }
    double getH() const { return m_h; }
    double getTau() const { return m_tau; }
    int getN() const { return m_n; }
    QString getDist() const {return m_distributionType;}

private:
    int64_t generateXi();

    double m_M;           // длина отрезка
    double m_h;           // длина маленького отрезка
    double m_tau;         // шаг значений
    int m_n;              // количество возможных значений (0..n)
    int m_stepCount;      // количество маленьких отрезков

    std::vector<double> m_values;  // значения ξ на каждом отрезке
    std::mt19937_64 m_rng;

    QString m_distributionType;
    QVariantMap m_distributionParams;
};
#endif // STEPFIGURE_HPP
