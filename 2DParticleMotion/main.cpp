#include <iostream>
#include <fstream>
#include <map>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>
#include <ostream>
#include "simulation.hpp"

#define EPSILON 1e-8

struct Config {
    double step_size = 1.0;
    double p = 0.5;
    int num_trials = 10000;
    uint64_t max_n = 1ULL << 32;
};

Config loadConfig(const QString& filename) 
{
    Config cfg;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) 
    {
        std::cerr << "Cannot open config file, using defaults.\n";
        return cfg;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    if (root.contains("step_size")) cfg.step_size = root["step_size"].toDouble();
    if (root.contains("p")) cfg.p = root["p"].toDouble();
    if (root.contains("num_trials")) cfg.num_trials = root["num_trials"].toInt();
    if (root.contains("max_n")) cfg.max_n = root["max_n"].toInt();

    return cfg;
}

int main(int argc, char* argv[]) 
{
    QString configFile = "config.json";
    if (argc > 1) configFile = argv[1];

    Config cfg = loadConfig(configFile);

    std::map<int, int> results;
    Simulation sim(cfg.step_size, cfg.p);

    for (int i = 0; i < cfg.num_trials; ++i) {
        int n = sim.runUntilReturnToZero();
        if (n > EPSILON && n <= cfg.max_n) results[n]++;
        std:: cout << "i = "<< i << "n = " << n << std::endl;
    }

    std::cout << "=== RESULTS ===\n";
    std::cout << "step_size = " << cfg.step_size << "\n";
    std::cout << "p = " << cfg.p << ", q = " << 1 - cfg.p << "\n";
    std::cout << "trials = " << cfg.num_trials << "\n\n";
    std::cout << "N\tProbability\n";

    for (const auto& [n, count] : results) 
    {
        double prob = static_cast<double>(count) / cfg.num_trials;
        std::cout << n << "\t" << prob << "\n";
    }

    return 0;
}