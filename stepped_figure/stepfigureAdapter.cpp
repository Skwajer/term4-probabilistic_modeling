#include "stepfigureAdapter.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

StepFigureAdapter::StepFigureAdapter(QObject *parent)
    : QObject(parent)
    , m_M(10.0)
    , m_h(1.0)
    , m_tau(1.0)
    , m_n(5)
    , m_increasingCount(0)
    , m_currentIndex(0)
    , m_rng(std::random_device{}())
{}

void StepFigureAdapter::configure(double M, double h, double tau, int n,
                                  const QString& distType, const QVariantMap& distParams)
{
    m_M = M;
    m_h = h;
    m_tau = tau;
    m_n = n;
    m_distType = distType;
    m_distParams = distParams;

    emit MChanged();
    emit hChanged();
    emit tauChanged();
    emit nChanged();
    emit statsChanged();
}

void StepFigureAdapter::generateMultiple(int count)
{
    qDebug() << "\n=== GENERATING" << count << "FIGURES ===";
    m_figures.clear();
    m_increasingCount = 0;

    for (int i = 0; i < count; i++) {
        StepFigure fig(m_M, m_h, m_tau, m_n);
        fig.setDistribution(m_distType, m_distParams);
        fig.generate();

        auto values = fig.getValues();
        m_figures.push_back(values);

        bool increasing = fig.isStrictlyIncreasing();
        if (increasing) {
            m_increasingCount++;
        }

        // Отладка первых 5 фигур
        if (i < 5) {
            qDebug() << "Figure" << i << "values:" << values;
            qDebug() << "  increasing:" << increasing;
        }
    }

    qDebug() << "Total increasing:" << m_increasingCount << "/" << count;
    qDebug() << "Probability:" << (double)m_increasingCount / count * 100 << "%";

    m_currentIndex = 0;
    updateCurrentFigure();
    emit statsChanged();
    emit currentIndexChanged();
}

void StepFigureAdapter::nextPage()
{
    int newIndex = m_currentIndex + 10;
    if (newIndex < static_cast<int>(m_figures.size())) {
        m_currentIndex = newIndex;
        updateCurrentFigure();
        emit currentIndexChanged();
    }
}

void StepFigureAdapter::prevPage()
{
    int newIndex = m_currentIndex - 10;
    if (newIndex >= 0) {
        m_currentIndex = newIndex;
        updateCurrentFigure();
        emit currentIndexChanged();
    }
}

void StepFigureAdapter::goToPage(int index)
{
    int newIndex = index * 10;
    if (newIndex >= 0 && newIndex < static_cast<int>(m_figures.size())) {
        m_currentIndex = newIndex;
        updateCurrentFigure();
        emit currentIndexChanged();
    }
}

void StepFigureAdapter::updateCurrentFigure()
{
    emit figureChanged();
}

QVariantList StepFigureAdapter::getCurrentFigure() const
{
    QVariantList list;

    if (m_figures.empty() || m_currentIndex >= static_cast<int>(m_figures.size())) {
        qDebug() << "No figures or invalid index";
        return list;
    }

    const auto& fig = m_figures[m_currentIndex];
    qDebug() << "Getting figure at index" << m_currentIndex << "size:" << fig.size();

    for (size_t i = 0; i < fig.size(); i++) {
        QVariantMap point;
        point["x"] = i * m_h;
        point["y"] = fig[i];
        list.append(point);
    }

    return list;
}

bool StepFigureAdapter::loadFromFile(const QString& filename)
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

    double M = root.value("M").toDouble(10.0);
    double h = root.value("h").toDouble(1.0);
    double tau = root.value("tau").toDouble(1.0);
    int n = root.value("n").toInt(5);
    QString distType = root.value("distribution").toString("uniform");

    QVariantMap distParams;
    if (root.contains("distribution_params")) {
        QJsonObject params = root["distribution_params"].toObject();
        for (auto it = params.begin(); it != params.end(); ++it) {
            distParams[it.key()] = it.value().toVariant();
        }
    }

    configure(M, h, tau, n, distType, distParams);

    return true;
}
