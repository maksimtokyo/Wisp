#ifndef VPNSERVICE_H
#define VPNSERVICE_H

#include <QProcess>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class Vpnservice : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Vpnservice)

    Q_PROPERTY(QString vlessurl READ vlessurl WRITE setvlessurl NOTIFY vlessurlChanged)

public:
    Vpnservice (QObject *parent = nullptr);

    QString vlessurl() {return m_vlessurl;}
    void setvlessurl(const QString &vlessurl);
    QString qetxraypath();

signals:
    void vlessurlChanged();
    void logmodelChanged();
public slots:
    void generatexrayconfig();
    void stas()
    {
        qDebug() << "Error code:" << process->state();
    }
private:
    QProcess *process = nullptr;
    QString m_config;
    QString m_vlessurl;
};

#endif // VPNSERVICE_H
