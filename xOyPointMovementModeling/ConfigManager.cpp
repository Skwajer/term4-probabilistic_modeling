#include "ConfigManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {}

bool ConfigManager::loadFromFile(const QString& fileName)
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
    if (root.contains("l")) setL(root["l"].toInt(0));

    if (root.contains("distribution")) {
        setDistributionType(root["distribution"].toString());
    }

    if (root.contains("s") && root["s"].isArray()) {
        QJsonArray sArray = root["s"].toArray();
        QVector<double> newS;
        for (int i = 0; i < sArray.size(); i++) {
            newS.append(sArray[i].toDouble());
        }
        setS(newS);
    }

    emit configLoaded();
    return true;
}

bool ConfigManager::saveToFile(const QString& fileName)
{
    QJsonObject root;
    root["h"] = m_h;
    root["n"] = m_n;
    root["K"] = m_K;
    root["l"] = m_l;
    root["distribution"] = m_distributionType;

    QJsonArray sArray;
    for (double val : m_s) {
        sArray.append(val);
    }
    root["s"] = sArray;

    QJsonDocument doc(root);
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write file:" << fileName;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

QVariantList ConfigManager::getSVariant() const
{
    QVariantList list;
    for (double val : m_s) {
        list.append(val);
    }
    return list;
}

void ConfigManager::setH(double h)
{
    if (qFuzzyCompare(m_h, h)) return;
    m_h = h;
    emit hChanged();
}

void ConfigManager::setN(int n)
{
    if (m_n == n) return;
    m_n = n;
    emit nChanged();
}

void ConfigManager::setK(int K)
{
    if (m_K == K) return;
    m_K = K;
    emit KChanged();
}

void ConfigManager::setL(int l)
{
    if (m_l == l) return;
    m_l = l;
    emit lChanged();
}

void ConfigManager::setDistributionType(const QString& type)
{
    if (m_distributionType == type) return;
    m_distributionType = type;
    emit distributionTypeChanged();
}

void ConfigManager::setS(const QVector<double>& s)
{
    m_s = s;
    emit sChanged();
}
