#include "simulationAdapter.hpp"


simulationAdapter::simulationAdapter(QObject *parent)
    : QObject(parent)
{
    m_simulation.setStepCallback([this]()
    {
        // using QMetaObject::invokeMethod for swtich on GUI stream
        QMetaObject::invokeMethod(this, "onSimulationStep", Qt::QueuedConnection);
    });

    m_timer.setInterval(100); // 100 milliseconds
    connect(&m_timer, &QTimer::timeout, this, &simulationAdapter::onTimerTimeout);
}

void simulationAdapter::onTimerTimeout()
{
    m_simulation.setP1(getP1());
    m_simulation.setP2(getP2());
    m_simulation.step();

    m_timeStep++;
    emit TimeStepChanged();
    emit statsChanged();
    emit simulationStepDone();
}



void simulationAdapter::loadFromFile(const QString& path)
{
    qDebug() << "Загрузка файла:" << path;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Ошибка: не могу открыть файл";
        return;
    }

    QTextStream in(&file);

    int numPeople;
    in >> numPeople;
    qDebug() << "Количество людей:" << numPeople;

    std::vector<std::pair<int, int>> edges;
    int id1, id2;

    while (in >> id1 >> id2) {
        edges.push_back({id1, id2});
        qDebug() << "Связь:" << id1 << "-" << id2;
    }

    file.close();

    if (m_simulation.loadGraph(numPeople, edges))
    {
        qDebug() << "Граф загружен успешно";
        emit statsChanged();
    } else {
        qDebug() << "Ошибка загрузки графа";
    }
}

void simulationAdapter::runSimulation()
{
    m_simulation.run();

    m_timeStep = 0;

    if (!m_running) {
        m_running = true;
        m_timer.start();
        emit runningChanged();
    }

    emit TimeStepChanged();
    emit statsChanged();
    emit simulationStepDone();
}

void simulationAdapter::pauseSimulation()
{
    if (m_running == true)
    {
        m_timer.stop();
        m_running = false;
        emit runningChanged();
    }
    else
    {
        m_timer.start();
        m_running = true;
        emit runningChanged();
    }
}

void simulationAdapter::resetSimulation()
{
    m_timer.stop();
    m_timeStep = 0;
    m_running = false;
    emit runningChanged();
    m_simulation.reset();
    emit statsChanged();
    emit TimeStepChanged();
}

QVector<int> simulationAdapter::findNeverInfected()
{
    QVector<int> healthy;
    for (int i = 0; i < m_simulation.getPersonCount(); i++)
    {
        if (m_simulation.getPerson(i)->state == State::Susceptible)
        {
            healthy.push_back(i);
        }
    }
    return healthy;
}
QVector<int> simulationAdapter::findRecovered()
{
    QVector<int> recovered;
    for (int i = 0; i < m_simulation.getPersonCount(); i++)
    {
        if (m_simulation.getPerson(i)->state == State::Recovered)
        {
            recovered.push_back(i);
        }
    }
    return recovered;
}

