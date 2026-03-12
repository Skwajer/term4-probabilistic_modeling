#ifndef SIMULATIONADAPTER_HPP
#define SIMULATIONADAPTER_HPP

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "simulation.hpp"


class SimulationAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double currentX READ getCurrentX NOTIFY trajectoryChanged)
    Q_PROPERTY(double currentY READ getCurrentY NOTIFY trajectoryChanged)
    Q_PROPERTY(int intersectionCount READ getIntersectionCount NOTIFY intersectionCountChanged)
    Q_PROPERTY(QVariantList trajectory READ getTrajectory NOTIFY trajectoryChanged)
    Q_PROPERTY(int n READ getN WRITE setN NOTIFY nChanged)
    Q_PROPERTY(int h READ getH WRITE setH NOTIFY hChanged)
    Q_PROPERTY(int k READ getK WRITE setK NOTIFY kChanged)
    Q_PROPERTY(QString distStr READ getDistStr WRITE setDistStr NOTIFY distStrChanged)
    Q_PROPERTY(bool running READ getRunning NOTIFY kChanged)

public:
    explicit SimulationAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void configure(double h, int n, int K, int l, QVariantList s);
    Q_INVOKABLE void setDistribution(const QString& type, QVariantMap params);
    Q_INVOKABLE void runSimulation();
    Q_INVOKABLE void resetSimulation();
    Q_INVOKABLE void dostep();
    Q_INVOKABLE void pauseSimulation();

    double getCurrentX() const { return m_simulation.getCurrentX(); }
    double getCurrentY() const { return m_simulation.getCurrentY(); }
    int getIntersectionCount() const { return m_simulation.getIntersectionCount(); }
    QVariantList getTrajectory() const;

    //getters
    int getN() { return m_simulation.getN();}
    int getH() {return m_simulation.getH();}
    int getK() {return m_simulation.getK();}
    QString getDistStr() {return m_simulation.getDist();}
    bool getRunning() const {return m_running;}
    //setters
    void setN(int n) {m_simulation.setN(n);}
    void setH(int h) {m_simulation.setH(h);}
    void setK(int k) {m_simulation.setK(k);}
    void setDistStr(QString const &dist_str) {m_simulation.setDistType(dist_str);}
    void setS(QVector<double> new_S) {m_simulation.setS(new_S);}

    Q_INVOKABLE bool loadFromFile(QString const &filename);

signals:
    void trajectoryChanged();
    void intersectionCountChanged();
    void simulationFinished();
    void nChanged();
    void hChanged();
    void kChanged();
    void distStrChanged();
    void configLoaded();


private:
    Simulation m_simulation;
    QTimer m_timer;
    bool m_running = false;
};

#endif // SIMULATIONADAPTER_HPP
