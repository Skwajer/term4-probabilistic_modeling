#include "tree_realization.hpp"
#include <cmath>
#include <QDebug>

MTreeModel::MTreeModel(QObject *parent)
    : QObject(parent)
    , m_m(2)
    , m_height(3)
    , m_currentNodeId(0)
    , m_stopProb(0.1)
    , m_currentDistribution(Uniform)
{
    qDebug() << "MTreeModel constructor called";
    buildTree();
    calculatePositions();
    calculateProbabilities(m_m);
    updateNodeProbabilities();
    updatePosition();

    qDebug() << "Модель инициализирована, узлов:" << m_nodes.size();
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

void MTreeModel::buildTree()
{
    m_nodes.clear();

    int totalNodes = 0;
    for (int i = 0; i <= m_height; ++i) {
        totalNodes += static_cast<int>(pow(m_m, i));
    }

    m_nodes.resize(totalNodes);
    qDebug() << "Строим дерево. M=" << m_m << "высота=" << m_height << "узлов=" << totalNodes;

    m_nodes[0] = Node(0, 0, m_stopProb, (m_height == 0));

    int nextId = 1;

    for (int level = 0; level < m_height; level++) {
        int nodesOnLevel = static_cast<int>(pow(m_m, level));
        int firstIdOnLevel = 0;

        for (int l = 0; l < level; l++) {
            firstIdOnLevel += static_cast<int>(pow(m_m, l));
        }

        for (int i = 0; i < nodesOnLevel; i++) {
            int parentId = firstIdOnLevel + i;

            for (int c = 0; c < m_m; c++) {
                if (nextId >= totalNodes) break;

                bool isLeaf = (level == m_height - 1);
                double stopProb = m_stopProb * (level + 2.0) / (m_height + 1.0);

                m_nodes[nextId] = Node(nextId, level + 1, stopProb, isLeaf);
                m_nodes[parentId].childs.append(nextId);

                nextId++;
            }
        }
    }

    qDebug() << "Дерево построено. Корень имеет детей:" << m_nodes[0].childs.size();
}

void MTreeModel::calculatePositions()
{
    int width = 1200;
    int height = 800;
    int totalNodes = m_nodes.size();

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
    return QString("Узел %1 (ур.%2, %3, p=%4)")
        .arg(node.id)
        .arg(node.level)
        .arg(type)
        .arg(node.stopP, 0, 'f', 2);
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
        qDebug() << "Установлено распределение:" << distributionType();
    }
}

void MTreeModel::setStopProbability(double prob)
{
    if (prob >= 0.0 && prob <= 1.0) {
        m_stopProb = prob;
        for (int i = 0; i < m_nodes.size(); ++i) {
            m_nodes[i].stopP = prob * (m_nodes[i].level + 1.0) / (m_height + 1.0);
        }
        emit stopProbabilityChanged();
        qDebug() << "Установлена вероятность остановки:" << prob;
    }
}

void MTreeModel::moveToChild(int childIndex)
{
    if (m_currentNodeId < 0 || m_currentNodeId >= m_nodes.size()) {
        qDebug() << "Ошибка: неверный ID узла";
        return;
    }

    Node& currentNode = m_nodes[m_currentNodeId];

    if (currentNode.isLeaf) {
        qDebug() << "Достигнут лист" << m_currentNodeId << "- остановка";
        return;
    }

    double randStop = QRandomGenerator::global()->generateDouble();
    if (randStop < currentNode.stopP) {
        qDebug() << "Остановка в узле" << m_currentNodeId << "с вероятностью" << currentNode.stopP;
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

            if (m_nodes[m_currentNodeId].isLeaf) {
                qDebug() << "Достигнут лист" << m_currentNodeId;
            } else {
                qDebug() << "Переместились в узел" << m_currentNodeId;
            }
        }
    }
}

void MTreeModel::resetPosition()
{
    m_currentNodeId = 0;
    updatePosition();
    emit currentNodeChanged();
    qDebug() << "Сброс позиции в корень";
}

void MTreeModel::rebuildTree(int m, int height)
{
    qDebug() << "========== ПЕРЕСТРОЙКА ДЕРЕВА ==========";
    qDebug() << "M=" << m << "высота=" << height;
    qDebug() << "Старые параметры: M=" << m_m << "высота=" << m_height;

    m_m = m;
    m_height = height;
    m_currentNodeId = 0;

    buildTree();
    calculatePositions();
    calculateProbabilities(m_m);
    updateNodeProbabilities();
    updatePosition();

    qDebug() << "Новое дерево создано. Узлов:" << m_nodes.size();
    qDebug() << "Позиций:" << m_nodePositions.size();
    qDebug() << "Корень имеет детей:" << (m_nodes.isEmpty() ? 0 : m_nodes[0].childs.size());

    emit treeChanged();
    emit treeRebuilt(); 
    emit currentNodeChanged();
    emit positionChanged();

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
