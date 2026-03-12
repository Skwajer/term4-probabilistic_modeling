#ifndef SIMULATION_HPP
#define SIMULATION_HPP


#include <random>
#include <vector>
#include <QString>
#include <QVariantMap>
#include <chrono>
#include <ctime>
#include <thread>

class Simulation
{
public:
    Simulation();

    void configure(double h, int n, int K, int l, const QVector<double>& s);
    void setDistribution(const QString& type, const QVariantMap& params);
    void run();
    void step();
    void reset();

    //getters
    double getCurrentX() const { return m_currentX; }
    double getCurrentY() const { return m_currentY; }
    const std::vector<double>& getTrajectory() const { return m_trajectory; }
    int getIntersectionCount() const { return m_intersectionCount; }
    int getN() { return m_n;}
    int getH() {return m_h;}
    int getK() {return m_K;}
    QString getDist() {return m_distributionType;}
    //setters
    void setN(int n) {m_n = n;}
    void setH(int h) {m_h = h;}
    void setK(int K) {m_K = K;}
    void setDistType(QString const &dist) {m_distributionType = dist;}
    void setS(QVector<double> new_S) {m_s = new_S;}


private:
    double generateRandomStep();

    std::mt19937 m_rng;
    std::uniform_real_distribution<> m_uniformDist;

    double m_h = 1.0;
    int m_n = 100;
    int m_K = 10;
    int m_l = 0;
    QVector<double> m_s;
    double m_currentX = 0.0;
    double m_currentY = 0.0;
    std::vector<double> m_trajectory;
    int m_intersectionCount = 0;

    QString m_distributionType = "uniform";
    QVariantMap m_distributionParams;
};

#endif // SIMULATION_HPP
