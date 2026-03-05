#ifndef TREE_REALIZATION_HPP
#define TREE_REALIZATION_HPP

#include <QObject>
#include <QVector>
#include <QRandomGenerator>
#include <QDebug>
#include <QPointF>
#include <cmath>

struct Node {
    int id;
    int level;
    double stopP;
    QVector<double> probs;
    QVector<int> childs;
    bool isLeaf;

    Node() : id(-1), level(0), stopP(0.1), isLeaf(false) {}
    Node(int _id, int _level, double _stopP, bool _isLeaf)
        : id(_id), level(_level), stopP(_stopP), isLeaf(_isLeaf) {}
};

class MTreeModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QPointF currentPosition READ currentPosition NOTIFY positionChanged)
    Q_PROPERTY(int treeHeight READ treeHeight NOTIFY treeChanged) 
    Q_PROPERTY(int mValue READ mValue NOTIFY treeChanged) 
    Q_PROPERTY(QString currentNodeInfo READ currentNodeInfo NOTIFY currentNodeChanged)
    Q_PROPERTY(double stopProbability READ stopProbability NOTIFY stopProbabilityChanged)
    Q_PROPERTY(QString distributionType READ distributionType NOTIFY distributionChanged)

public:
    explicit MTreeModel(QObject *parent = nullptr);

    enum Distribution {
        Uniform = 0,
        Geometric,
        Increasing
    };
    Q_ENUM(Distribution)

   
    QPointF currentPosition() const { return m_currentPosition; }
    int treeHeight() const { return m_height; }
    int mValue() const { return m_m; }
    QString currentNodeInfo() const;
    double stopProbability() const { return m_stopProb; }
    QString distributionType() const;


    Q_INVOKABLE void setDistribution(int dist);
    Q_INVOKABLE void setStopProbability(double prob);
    Q_INVOKABLE void moveToChild(int childIndex);
    Q_INVOKABLE void resetPosition();
    Q_INVOKABLE void rebuildTree(int m, int height);
    Q_INVOKABLE QPointF getNodePosition(int nodeId) const;
    Q_INVOKABLE int nodeCount() const { return m_nodes.size(); }
    Q_INVOKABLE bool isLeaf(int nodeId) const;
    Q_INVOKABLE int getCurrentNodeId() const { return m_currentNodeId; }

signals:
    void positionChanged();
    void currentNodeChanged();
    void stopProbabilityChanged();
    void distributionChanged();
    void treeChanged(); 
    void treeRebuilt(); 

private:
    void buildTree();
    void calculatePositions();
    void updatePosition();
    void calculateProbabilities(int childCount);
    void updateNodeProbabilities();

    int m_m;              
    int m_height;            
    int m_currentNodeId;       
    double m_stopProb;         

    QVector<Node> m_nodes;       
    Distribution m_currentDistribution;
    QVector<double> m_currentProbs; 
    QVector<QPointF> m_nodePositions; 
    QPointF m_currentPosition; 
};

#endif // TREE_REALIZATION_HPP
