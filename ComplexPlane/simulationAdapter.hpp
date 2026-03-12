#ifndef SIMULATIONADAPTER_HPP
#define SIMULATIONADAPTER_HPP

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <QVector>
#include <complex>
#include "simulation.hpp"

struct ReturnRecord {
    int trialNumber;
    int stepsToReturn;
    double finalX;
    double finalY;
};

class SimulationAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList trajectory READ getTrajectory NOTIFY trajectoryChanged)
    Q_PROPERTY(bool returned READ getReturned NOTIFY simulationFinished)
    Q_PROPERTY(int stepsToReturn READ getStepsToReturn NOTIFY simulationFinished)
    Q_PROPERTY(double rho READ getRho WRITE setRho NOTIFY rhoChanged)
    Q_PROPERTY(int n READ getN NOTIFY nChanged)
    Q_PROPERTY(int K READ getK NOTIFY KChanged)
    Q_PROPERTY(int M READ getM NOTIFY MChanged)
    Q_PROPERTY(QString distType READ getDistType NOTIFY distTypeChanged)
    Q_PROPERTY(int totalReturns READ getTotalReturns NOTIFY statsUpdated)
    Q_PROPERTY(int totalTrials READ getTotalTrials NOTIFY statsUpdated)
    Q_PROPERTY(int currentTrial READ getCurrentTrial NOTIFY currentTrialChanged)
    Q_PROPERTY(int maxTrials READ getMaxTrials NOTIFY maxTrialsChanged)
    Q_PROPERTY(int xiCount READ getXiCount NOTIFY xiValuesChanged)

public:
    explicit SimulationAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void configure(double rho, int n, int K, int M, const QString& distType, const QVariantMap& distParams);
    Q_INVOKABLE void runSingleSimulation(int maxSteps = 1000000);
    Q_INVOKABLE void runMultipleSimulations(int numTrials, int maxSteps = 1000000);
    Q_INVOKABLE void stepSimulation();
    Q_INVOKABLE void reset();
    Q_INVOKABLE bool loadFromFile(const QString& filename);
    Q_INVOKABLE QVariantList getReturnRecords() const;

    QVariantList getTrajectory() const;
    bool getReturned() const { return m_simulation.returnedToOrigin(); }
    int getStepsToReturn() const { return m_simulation.getStepsToReturn(); }
    double getRho() const { return m_rho; }
    int getN() const { return m_n; }
    int getK() const { return m_K; }
    int getM() const { return m_M; }
    QString getDistType() const { return m_distType; }
    int getTotalReturns() const { return m_totalReturns; }
    int getTotalTrials() const { return m_totalTrials; }
    int getCurrentTrial() const { return m_currentTrial; }
    int getMaxTrials() const { return m_totalTrials; }
    int getXiCount() const { return m_xiValues.size(); }

signals:
    void trajectoryChanged();
    void simulationFinished();
    void rhoChanged();
    void nChanged();
    void KChanged();
    void MChanged();
    void distTypeChanged();
    void statsUpdated();
    void currentTrialChanged();
    void maxTrialsChanged();
    void xiValuesChanged();

public slots:
    void setRho(double rho);

private:
    Simulation m_simulation;
    std::vector<std::complex<double>> m_currentTrajectory;
    QVector<ReturnRecord> m_returnRecords;
    QTimer m_timer;

    double m_rho;
    int m_n;
    int m_K;
    int m_M;
    QString m_distType;
    QVariantMap m_distParams;
    std::vector<int64_t> m_xiValues;

    int m_totalReturns;
    int m_totalTrials;
    int m_currentTrial;
    int m_maxSteps;
};

#endif // SIMULATIONADAPTER_HPP
