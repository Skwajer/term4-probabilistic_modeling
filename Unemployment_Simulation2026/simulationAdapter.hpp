#ifndef SIMULATIONADAPTER_HPP
#define SIMULATIONADAPTER_HPP

#include <QObject>
#include <QVariantList>
#include <QVector>
#include <QTimer>
#include <QFile>

#include "simulation.hpp"

class simulationAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double p1 READ getP1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(double p2 READ getP2 WRITE setP2 NOTIFY p2Changed)
    Q_PROPERTY(int recoveredCount READ recoveredCount NOTIFY statsChanged)
    Q_PROPERTY(int healthyCount READ healthyCount NOTIFY statsChanged)
    Q_PROPERTY(int infectiousCount READ infectiousCount NOTIFY statsChanged)
    Q_PROPERTY(int totalPeople READ totalPeople NOTIFY statsChanged)
    Q_PROPERTY(int TimeStep READ TimeStep NOTIFY TimeStepChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)


public:
    explicit simulationAdapter(QObject *parent = nullptr);
    double getP1() const { return m_simulation.getP1(); }
    double getP2() const { return m_simulation.getP2();}
    void setP1(double new_p1) {m_simulation.setP1(new_p1); emit p1Changed();    qDebug() << "setP1 called with value:" << new_p1;}
    void setP2(double new_p2) {m_simulation.setP2(new_p2); emit p2Changed();}
    bool running() const { return m_running; }
    //void setrunning() {}
    int TimeStep() const { return m_timeStep; }

    int healthyCount() const { return m_simulation.getSusceptibleCount(); }
    int infectiousCount() const { return m_simulation.getInfectedCount(); }
    int recoveredCount() const { return m_simulation.getRecoveredCount(); }
    int totalPeople() const { return m_simulation.getPersonCount(); }

signals:
    void p1Changed();
    void p2Changed();
    void statsChanged();
    void runningChanged();
    void TimeStepChanged();
    void simulationStepDone();

public slots:
    void onTimerTimeout();
    void loadFromFile(const QString& path);
    void runSimulation();
    void pauseSimulation();
    void resetSimulation();
    QVector<int> findNeverInfected();
    QVector<int> findRecovered();

private:
    Simulation m_simulation;
    QTimer m_timer;
    bool m_running = false;
    int m_timeStep = 0;
};
#endif // SIMULATIONADAPTER_HPP
