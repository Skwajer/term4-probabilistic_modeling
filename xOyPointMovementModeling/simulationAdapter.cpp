#include "simulationAdapter.hpp"

SimulationAdapter::SimulationAdapter(QObject *parent) : QObject(parent)
{
    m_timer.setInterval(300);
    m_timer.setTimerType(Qt::PreciseTimer);
    connect(&m_timer, &QTimer::timeout, this, &SimulationAdapter::dostep);
}

void SimulationAdapter::configure(double h, int n, int K, int l, QVariantList s)
{
    QVector<double> sVector;
    for (const auto& val : s) {
        sVector.push_back(val.toDouble());
    }
    m_simulation.configure(h, n, K, l, sVector);
}

void SimulationAdapter::setDistribution(const QString& type, QVariantMap params)
{
    m_simulation.setDistribution(type, params);
}

void SimulationAdapter::runSimulation()
{
    m_running = true;
    m_timer.start(300);
}

void SimulationAdapter::dostep()
{
        m_simulation.step();
        emit trajectoryChanged();
        emit intersectionCountChanged();
}

void SimulationAdapter::pauseSimulation()
{
    if (m_running)
    {
        m_running = false;
        m_timer.stop();
    }
    else
    {
        m_running = true;
        m_timer.start();
    }
}

void SimulationAdapter::resetSimulation()
{
    m_simulation.reset();
    m_timer.stop();
    m_running = false;
    emit trajectoryChanged();
    emit intersectionCountChanged();
}

QVariantList SimulationAdapter::getTrajectory() const
{
    QVariantList list;
    const auto& traj = m_simulation.getTrajectory();
    for (double val : traj) {
        list.append(val);
    }
    return list;
}

bool SimulationAdapter::loadFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << fileName;
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

    if (root.contains("h")) setH(root["h"].toDouble(1.0));
    if (root.contains("n")) setN(root["n"].toInt(100));
    if (root.contains("K")) setK(root["K"].toInt(10));
    //if (root.contains("l")) setL(root["l"].toInt(0));

    if (root.contains("distribution")) {
        setDistStr(root["distribution"].toString());
    }

    if (root.contains("s") && root["s"].isArray()) {
        QJsonArray sArray = root["s"].toArray();
        QVector<double> newS;
        for (int i = 0; i < sArray.size(); i++) {
            newS.append(sArray[i].toDouble());
        }
        setS(newS);
    }

    emit nChanged();
    emit hChanged();
    emit kChanged();
    emit distStrChanged();
    emit configLoaded();
    return true;
}
