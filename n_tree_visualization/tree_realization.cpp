#include "tree_realization.hpp"
#include <cmath>
#include <QDebug>

MTreeModel::MTreeModel(QObject *parent)
    : QObject(parent)
    , m_m(2)
    , m_height(3)
    , m_currentNodeId(0)
    , m_stopProb(0.1)
    , m_isStopped(false)
    , m_targetNodeId(-1)
    , m_targetProbability(0.0)
    , m_currentDistribution(Uniform)
{
    qDebug() << "MTreeModel constructor called";
    buildTree();
    calculatePositions();
    calculateProbabilities(m_m);
    updateNodeProbabilities();
    generateRandomStopProbabilities();
    updatePosition();

    qDebug() << "Модель инициализирована, узлов:" << m_nodes.size();
}

double MTreeModel::getNodeStopProbability(int nodeId) const
{
    if (nodeId >= 0 && nodeId < m_nodes.size()) {
        return m_nodes[nodeId].stopP;
    }
    return 0.0;
}

QVector<int> MTreeModel::getPathToNode(int nodeId) const
{
    QVector<int> path;
    if (nodeId < 0 || nodeId >= m_nodes.size()) {
        return path;
    }

    // Находим путь от корня к узлу
    QVector<int> stack;
    stack.append(0);
    QVector<int> parents(m_nodes.size(), -1);
    parents[0] = 0;

    while (!stack.isEmpty()) {
        int current = stack.takeLast();

        if (current == nodeId) {
            // Восстанавливаем путь
            int temp = nodeId;
            while (temp != 0) {
                path.prepend(temp);
                temp = parents[temp];
            }
            path.prepend(0);
            break;
        }

        const Node& node = m_nodes[current];
        for (int child : node.childs) {
            if (parents[child] == -1) {
                parents[child] = current;
                stack.append(child);
            }
        }
    }

    return path;
}

QString MTreeModel::formatPath(const QVector<int>& path) const
{
    if (path.isEmpty()) return "Нет пути";

    QString result;
    for (int i = 0; i < path.size(); ++i) {
        if (i > 0) result += " → ";
        result += QString::number(path[i]);
    }
    return result;
}

double MTreeModel::calculatePathProbability(const QVector<int>& path)
{
    if (path.size() < 2) return 1.0;

    double probability = 1.0;

    for (int i = 0; i < path.size() - 1; ++i) {
        int currentNode = path[i];
        int nextNode = path[i + 1];

        const Node& node = m_nodes[currentNode];

        // Вероятность НЕ остановиться в текущем узле
        probability *= (1.0 - node.stopP);

        // Вероятность выбрать правильного ребенка
        int childIndex = node.childs.indexOf(nextNode);
        if (childIndex >= 0 && childIndex < node.probs.size()) {
            probability *= node.probs[childIndex];
        } else {
            return 0.0;
        }
    }

    return probability;
}

void MTreeModel::calculatePathProbability()
{
    if (m_targetNodeId < 0 || m_targetNodeId >= m_nodes.size()) {
        m_targetProbability = 0.0;
        emit targetProbabilityCalculated();
        return;
    }

    QVector<int> path = getPathToNode(m_targetNodeId);
    if (path.isEmpty()) {
        m_targetProbability = 0.0;
        emit targetProbabilityCalculated();
        return;
    }

    m_targetProbability = calculatePathProbability(path);
    qDebug() << "Вероятность попадания в узел" << m_targetNodeId
             << ":" << m_targetProbability;
    emit targetProbabilityCalculated();
}

void MTreeModel::setTargetNodeId(int nodeId)
{
    if (nodeId >= -1 && nodeId < m_nodes.size()) {
        m_targetNodeId = nodeId;
        calculatePathProbability();
        emit targetChanged();
    }
}

QString MTreeModel::targetInfo() const
{
    if (m_targetNodeId < 0 || m_targetNodeId >= m_nodes.size()) {
        return "Цель не выбрана";
    }

    QVector<int> path = getPathToNode(m_targetNodeId);
    QString pathStr = formatPath(path);

    const Node& node = m_nodes[m_targetNodeId];
    QString type = node.isLeaf ? "🍃 лист" : "📁 внутр.";

    return QString("Цель: \n"
                   "Путь: \n"
                   "Вероятность: ")
        .arg(m_targetNodeId)
        .arg(type)
        .arg(node.level)
        .arg(pathStr)
        .arg(m_targetProbability, 0, 'f', 4);
}

void MTreeModel::calculateProbabilities(int childCount)
{
    if (childCount <= 0) return;

    m_currentProbs.clear();
    m_currentProbs.resize(childCount);

    switch (m_currentDistribution)
    {
    case Uniform: {
        double prob = 1.0 / childCount;
        for (int i = 0; i < childCount; ++i) {
            m_currentProbs[i] = prob;
        }
        break;
    }

    case Geometric:
    {
        double q = 0.5;
        double sum = 0.0;

        for (int i = 0; i < childCount; ++i) {
            m_currentProbs[i] = (1 - q) * pow(q, i);
            sum += m_currentProbs[i];
        }

        for (int i = 0; i < childCount; ++i) {
            m_currentProbs[i] /= sum;
        }
        break;
    }

    case Increasing:
    {
        double sum = 0.0;

        for (int i = 0; i < childCount; ++i) {
            m_currentProbs[i] = i + 1.0;
            sum += m_currentProbs[i];
        }

        for (int i = 0; i < childCount; ++i) {
            m_currentProbs[i] /= sum;
        }
        break;
    }
    }
}

void MTreeModel::updateNodeProbabilities()
{
    for (int i = 0; i < m_nodes.size(); ++i) {
        if (!m_nodes[i].isLeaf && m_nodes[i].childs.size() > 0) {
            m_nodes[i].probs = m_currentProbs;
        }
    }
}

void MTreeModel::generateRandomStopProbabilities()
{
    for (int i = 0; i < m_nodes.size(); ++i) {
        double randomProb = QRandomGenerator::global()->generateDouble() * m_stopProb;
        double levelFactor = (m_nodes[i].level + 1.0) / (m_height + 1.0);
        randomProb = randomProb * (0.5 + levelFactor * 0.5);
        m_nodes[i].stopP = qMin(randomProb, 1.0);
    }

    qDebug() << "Сгенерированы случайные вероятности остановки для" << m_nodes.size() << "узлов";
}

void MTreeModel::buildTree()
{
    m_nodes.clear();

    int totalNodes = 0;
    for (int i = 0; i <= m_height; ++i) {
        totalNodes += static_cast<int>(pow(m_m, i));
    }

    m_nodes.resize(totalNodes);
    qDebug() << "Строим дерево. M=" << m_m << "высота=" << m_height << "узлов=" << totalNodes;

    if (totalNodes > 0) {
        m_nodes[0] = Node(0, 0, 0.0, (m_height == 0));
    }

    int nextId = 1;

    for (int level = 0; level < m_height; level++) {
        int nodesOnLevel = static_cast<int>(pow(m_m, level));
        int firstIdOnLevel = 0;

        for (int l = 0; l < level; l++) {
            firstIdOnLevel += static_cast<int>(pow(m_m, l));
        }

        for (int i = 0; i < nodesOnLevel; i++) {
            int parentId = firstIdOnLevel + i;
            if (parentId >= m_nodes.size()) break;

            for (int c = 0; c < m_m; c++) {
                if (nextId >= totalNodes) break;

                bool isLeaf = (level == m_height - 1);
                double stopProb = 0.0;

                m_nodes[nextId] = Node(nextId, level + 1, stopProb, isLeaf);
                m_nodes[parentId].childs.append(nextId);

                nextId++;
            }
        }
    }

    qDebug() << "Дерево построено. Корень имеет детей:" << (m_nodes.isEmpty() ? 0 : m_nodes[0].childs.size());
}

void MTreeModel::calculatePositions()
{
    int width = 1200;
    int height = 800;
    int totalNodes = m_nodes.size();

    m_nodePositions.clear();
    m_nodePositions.resize(totalNodes);

    double levelHeight = static_cast<double>(height) / (m_height + 1);

    for (int level = 0; level <= m_height; level++) {
        int nodesOnLevel = static_cast<int>(pow(m_m, level));
        int firstIdOnLevel = 0;

        for (int l = 0; l < level; l++) {
            firstIdOnLevel += static_cast<int>(pow(m_m, l));
        }

        for (int i = 0; i < nodesOnLevel; i++) {
            int nodeId = firstIdOnLevel + i;

            if (nodeId < totalNodes) {
                double x = (i + 1.0) * width / (nodesOnLevel + 1.0);
                double y = (level + 1.0) * levelHeight;

                m_nodePositions[nodeId] = QPointF(x, y);
            }
        }
    }
}

void MTreeModel::updatePosition()
{
    if (m_currentNodeId >= 0 && m_currentNodeId < m_nodePositions.size()) {
        m_currentPosition = m_nodePositions[m_currentNodeId];
        emit positionChanged();
    }
}

QString MTreeModel::currentNodeInfo() const
{
    if (m_currentNodeId < 0 || m_currentNodeId >= m_nodes.size()) {
        return "Нет узла";
    }

    const Node& node = m_nodes[m_currentNodeId];
    QString type = node.isLeaf ? "🍃 лист" : "📁 внутр.";
    QString stopStatus = m_isStopped ? " [ОСТАНОВЛЕНО]" : "";

    return QString("Узел %1 (ур.%2, %3, p=%4)%5")
        .arg(node.id)
        .arg(node.level)
        .arg(type)
        .arg(node.stopP, 0, 'f', 3)
        .arg(stopStatus);
}

QString MTreeModel::distributionType() const
{
    switch (m_currentDistribution) {
    case Uniform: return "Равномерное";
    case Geometric: return "Геометрическое";
    case Increasing: return "Возрастающее";
    default: return "Неизвестно";
    }
}

void MTreeModel::setDistribution(int dist)
{
    if (dist >= 0 && dist <= 2) {
        m_currentDistribution = static_cast<Distribution>(dist);
        calculateProbabilities(m_m);
        updateNodeProbabilities();
        emit distributionChanged();
        if (m_targetNodeId >= 0) {
            calculatePathProbability();
        }
        qDebug() << "Установлено распределение:" << distributionType();
    }
}

void MTreeModel::setStopProbability(double prob)
{
    if (prob >= 0.0 && prob <= 1.0) {
        m_stopProb = prob;
        generateRandomStopProbabilities();
        emit stopProbabilityChanged();
        emit currentNodeChanged();
        if (m_targetNodeId >= 0) {
            calculatePathProbability();
        }
        qDebug() << "Установлена базовая вероятность остановки:" << prob;
    }
}

void MTreeModel::moveToChild(int childIndex)
{
    if (m_isStopped) {
        qDebug() << "Движение остановлено! Нажмите Reset для продолжения.";
        emit movementBlocked();
        return;
    }

    if (m_currentNodeId < 0 || m_currentNodeId >= m_nodes.size()) {
        qDebug() << "Ошибка: неверный ID узла";
        return;
    }

    Node& currentNode = m_nodes[m_currentNodeId];

    double randStop = QRandomGenerator::global()->generateDouble();

    if (randStop < currentNode.stopP) {
        qDebug() << "Остановка в узле" << m_currentNodeId
                 << "с вероятностью" << currentNode.stopP;

        m_isStopped = true;
        emit stoppedChanged();
        emit currentNodeChanged();
        return;
    }

    if (currentNode.isLeaf) {
        qDebug() << "Достигнут лист" << m_currentNodeId << "- остановка (лист)";
        m_isStopped = true;
        emit stoppedChanged();
        emit currentNodeChanged();
        return;
    }

    if (!currentNode.childs.isEmpty()) {
        int selectedChild = childIndex;

        if (childIndex < 0 || childIndex >= currentNode.childs.size()) {
            if (currentNode.probs.size() == currentNode.childs.size()) {
                double rand = QRandomGenerator::global()->generateDouble();
                double cumulative = 0.0;

                for (int i = 0; i < currentNode.probs.size(); ++i) {
                    cumulative += currentNode.probs[i];
                    if (rand < cumulative) {
                        selectedChild = i;
                        break;
                    }
                }
            } else {
                selectedChild = QRandomGenerator::global()->bounded(currentNode.childs.size());
            }
        }

        if (selectedChild >= 0 && selectedChild < currentNode.childs.size()) {
            m_currentNodeId = currentNode.childs[selectedChild];
            updatePosition();
            emit currentNodeChanged();

            if (m_currentNodeId == m_targetNodeId) {
                qDebug() << "🎯 ДОСТИГНУТА ЦЕЛЬ! Узел" << m_targetNodeId;
            }

            if (m_nodes[m_currentNodeId].isLeaf) {
                qDebug() << "Достигнут лист" << m_currentNodeId << "- автоматическая остановка";
                m_isStopped = true;
                emit stoppedChanged();
            } else {
                qDebug() << "Переместились в узел" << m_currentNodeId
                         << "(вероятность остановки:" << m_nodes[m_currentNodeId].stopP << ")";
            }
        }
    }
}

void MTreeModel::resetPosition()
{
    m_currentNodeId = 0;
    m_isStopped = false;
    updatePosition();
    emit currentNodeChanged();
    emit stoppedChanged();
    qDebug() << "Сброс позиции в корень. Движение разблокировано.";
}

void MTreeModel::rebuildTree(int m, int height)
{
    qDebug() << "========== ПЕРЕСТРОЙКА ДЕРЕВА ==========";
    qDebug() << "M=" << m << "высота=" << height;

    m_m = m;
    m_height = height;
    m_currentNodeId = 0;
    m_isStopped = false;
    m_targetNodeId = -1;
    m_targetProbability = 0.0;

    buildTree();
    calculatePositions();
    calculateProbabilities(m_m);
    updateNodeProbabilities();
    generateRandomStopProbabilities();
    updatePosition();

    qDebug() << "Новое дерево создано. Узлов:" << m_nodes.size();
    qDebug() << "Позиций:" << m_nodePositions.size();

    emit treeChanged();
    emit treeRebuilt();
    emit currentNodeChanged();
    emit positionChanged();
    emit stoppedChanged();
    emit targetChanged();

    qDebug() << "========== ПЕРЕСТРОЙКА ЗАВЕРШЕНА ==========";
}

QPointF MTreeModel::getNodePosition(int nodeId) const
{
    if (nodeId >= 0 && nodeId < m_nodePositions.size()) {
        return m_nodePositions[nodeId];
    }
    return QPointF(0, 0);
}

bool MTreeModel::isLeaf(int nodeId) const
{
    if (nodeId >= 0 && nodeId < m_nodes.size()) {
        return m_nodes[nodeId].isLeaf;
    }
    return false;
}
