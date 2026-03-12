#ifndef STEPFIGUREADAPTER_HPP
#define STEPFIGUREADAPTER_HPP


#include <QObject>
#include <QTimer>
#include <QVariantList>
#include "stepfigure.hpp"

class StepFigureAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList currentFigure READ getCurrentFigure NOTIFY figureChanged)
    Q_PROPERTY(double M READ getM NOTIFY MChanged)
    Q_PROPERTY(double h READ getH NOTIFY hChanged)
    Q_PROPERTY(double tau READ getTau NOTIFY tauChanged)
    Q_PROPERTY(int n READ getN NOTIFY nChanged)
    Q_PROPERTY(int totalFigures READ getTotalFigures NOTIFY statsChanged)
    Q_PROPERTY(int increasingCount READ getIncreasingCount NOTIFY statsChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int totalPages READ getTotalPages NOTIFY statsChanged)

public:
    explicit StepFigureAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void configure(double M, double h, double tau, int n,
                               const QString& distType, const QVariantMap& distParams);
    Q_INVOKABLE void generateMultiple(int count);
    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void prevPage();
    Q_INVOKABLE void goToPage(int index);
    Q_INVOKABLE bool loadFromFile(const QString& filename);

    QVariantList getCurrentFigure() const;
    double getM() const { return m_M; }
    double getH() const { return m_h; }
    double getTau() const { return m_tau; }
    int getN() const { return m_n; }
    int getTotalFigures() const { return m_figures.size(); }
    int getIncreasingCount() const { return m_increasingCount; }
    int getCurrentIndex() const { return m_currentIndex; }
    int getTotalPages() const { return (m_figures.size() + 9) / 10; } // 10 на страницу

signals:
    void figureChanged();
    void statsChanged();
    void currentIndexChanged();
    void MChanged();
    void hChanged();
    void tauChanged();
    void nChanged();

private:
    void updateCurrentFigure();

    double m_M;
    double m_h;
    double m_tau;
    int m_n;
    QString m_distType;
    QVariantMap m_distParams;

    std::vector<std::vector<double>> m_figures;
    int m_increasingCount;
    int m_currentIndex;

    std::mt19937_64 m_rng;
};

#endif // STEPFIGUREADAPTER_HPP
