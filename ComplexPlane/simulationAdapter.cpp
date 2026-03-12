#include "simulationAdapter.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

SimulationAdapter::SimulationAdapter(QObject *parent)
    : QObject(parent)
    , m_simulation(1.0, 8)
    , m_rho(1.0)
    , m_n(8)
    , m_K(10)
    , m_M(100)
    , m_distType("uniform")
    , m_totalReturns(0)
    , m_totalTrials(0)
    , m_currentTrial(0)
    , m_maxSteps(1000000)
{
    connect(&m_timer, &QTimer::timeout, this, &SimulationAdapter::stepSimulation);
}

void SimulationAdapter::configure(double rho, int n, int K, int M, const QString& distType, const QVariantMap& distParams)
{
    bool rhoChanged = (m_rho != rho);
    bool nChanged = (m_n != n);
    bool kChanged = (m_K != K);
    bool mChanged = (m_M != M);
    bool distChanged = (m_distType != distType);

    m_rho = rho;
    m_n = n;
    m_K = K;
    m_M = M;
    m_distType = distType;
    m_distParams = distParams;

    m_simulation = Simulation(rho, n);
    m_simulation.setDistribution(distType, distParams);
    m_simulation.setXiValues(m_xiValues);

     emit KChanged();
    emit MChanged();
     emit distTypeChanged();
}

void SimulationAdapter::runSingleSimulation(int maxSteps)
{
    m_simulation.setXiValues(m_xiValues);
    m_simulation.reset();
    m_simulation.run(maxSteps);

    m_currentTrajectory = m_simulation.getTrajectory();
    m_totalReturns = m_simulation.returnedToOrigin() ? 1 : 0;
    m_totalTrials = 1;

    emit trajectoryChanged();
    emit simulationFinished();
    emit statsUpdated();
}

void SimulationAdapter::runMultipleSimulations(int numTrials, int maxSteps)
{
    m_returnRecords.clear();
    m_totalReturns = 0;
    m_totalTrials = numTrials;
    m_currentTrial = 0;
    m_maxSteps = maxSteps;

    emit maxTrialsChanged();
    emit currentTrialChanged();
    emit statsUpdated();

    m_timer.start(50);
}

void SimulationAdapter::stepSimulation()
{
    if (m_currentTrial >= m_totalTrials) {
        m_timer.stop();
        emit statsUpdated();
        return;
    }

    // Создаем новую симуляцию для каждого испытания
    Simulation sim(m_rho, m_n);
    sim.setDistribution(m_distType, m_distParams);
    sim.setXiValues(m_xiValues);

    sim.run(m_maxSteps);

    if (sim.returnedToOrigin()) {
        m_totalReturns++;

        ReturnRecord record;
        record.trialNumber = m_currentTrial + 1;
        record.stepsToReturn = sim.getStepsToReturn();

        const auto& traj = sim.getTrajectory();
        if (!traj.empty()) {
            record.finalX = traj.back().real();
            record.finalY = traj.back().imag();
        }

        m_returnRecords.append(record);
    }

    m_currentTrial++;
    emit currentTrialChanged();
    emit statsUpdated();

    // Показываем каждую K-ю траекторию
    if (m_currentTrial % m_K == 0 || m_currentTrial == 1) {
        m_currentTrajectory = sim.getTrajectory();
        emit trajectoryChanged();
    }
}

void SimulationAdapter::reset()
{
    m_simulation.reset();
    m_currentTrajectory.clear();
    m_returnRecords.clear();
    m_totalReturns = 0;
    m_totalTrials = 0;
    m_currentTrial = 0;

    emit trajectoryChanged();
    emit statsUpdated();
    emit currentTrialChanged();
    emit maxTrialsChanged();
}

QVariantList SimulationAdapter::getTrajectory() const
{
    QVariantList list;

    const auto& traj = m_currentTrajectory.empty() ?
                           m_simulation.getTrajectory() :
                           m_currentTrajectory;

    for (const auto& point : traj) {
        QVariantMap map;
        map["x"] = point.real();
        map["y"] = point.imag();
        list.append(map);
    }
    return list;
}

QVariantList SimulationAdapter::getReturnRecords() const
{
    QVariantList list;
    for (const auto& record : m_returnRecords) {
        QVariantMap map;
        map["trial"] = record.trialNumber;
        map["steps"] = record.stepsToReturn;
        map["x"] = record.finalX;
        map["y"] = record.finalY;
        list.append(map);
    }
    return list;
}

void SimulationAdapter::setRho(double rho)
{
    if (qFuzzyCompare(m_rho, rho)) return;
    m_rho = rho;
    m_simulation = Simulation(rho, m_n);
    m_simulation.setDistribution(m_distType, m_distParams);
    m_simulation.setXiValues(m_xiValues);
    emit rhoChanged();
}

bool SimulationAdapter::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filename;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    QJsonObject root = doc.object();

    double rho = root.value("rho").toDouble(1.0);
    int n = root.value("n").toInt(8);
    int K = root.value("K").toInt(10);
    int M = root.value("M").toInt(100);
    QString distType = root.value("distribution").toString("uniform");

    QVariantMap distParams;
    if (root.contains("distribution_params")) {
        QJsonObject params = root["distribution_params"].toObject();
        for (auto it = params.begin(); it != params.end(); ++it) {
            distParams[it.key()] = it.value().toVariant();
        }
    }

    // Загружаем xi_values если есть
    m_xiValues.clear();
    if (root.contains("xi_values") && root["xi_values"].isArray()) {
        QJsonArray arr = root["xi_values"].toArray();
        for (const auto& val : arr) {
            m_xiValues.push_back(val.toInteger());
        }
    }
    emit xiValuesChanged();

    configure(rho, n, K, M, distType, distParams);

    return true;
}
