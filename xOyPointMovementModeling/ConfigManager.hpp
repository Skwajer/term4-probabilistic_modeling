#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include <QObject>
#include <QVector>
#include <QString>
#include <QJsonObject>

class ConfigManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double h READ getH WRITE setH NOTIFY hChanged)
    Q_PROPERTY(int n READ getN WRITE setN NOTIFY nChanged)
    Q_PROPERTY(int K READ getK WRITE setK NOTIFY KChanged)
    Q_PROPERTY(int l READ getL WRITE setL NOTIFY lChanged)
    Q_PROPERTY(QString distributionType READ getDistributionType WRITE setDistributionType NOTIFY distributionTypeChanged)
    Q_PROPERTY(QVariantList s READ getSVariant NOTIFY sChanged)

public:
    explicit ConfigManager(QObject *parent = nullptr);

    Q_INVOKABLE bool loadFromFile(const QString& fileName);
    Q_INVOKABLE bool saveToFile(const QString& fileName);

    double getH() const { return m_h; }
    int getN() const { return m_n; }
    int getK() const { return m_K; }
    int getL() const { return m_l; }
    QString getDistributionType() const { return m_distributionType; }
    QVector<double> getS() const { return m_s; }
    QVariantList getSVariant() const;

public slots:
    void setH(double h);
    void setN(int n);
    void setK(int K);
    void setL(int l);
    void setDistributionType(const QString& type);
    void setS(const QVector<double>& s);

signals:
    void hChanged();
    void nChanged();
    void KChanged();
    void lChanged();
    void distributionTypeChanged();
    void sChanged();
    void configLoaded();

private:
    double m_h = 1.0;
    int m_n = 100;
    int m_K = 10;
    int m_l = 0;
    QString m_distributionType = "uniform";
    QVector<double> m_s;
};

#endif // CONFIGMANAGER_HPP
