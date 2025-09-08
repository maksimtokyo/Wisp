#include "vpnservice.h"


Vpnservice::Vpnservice(QObject *parent) : QObject(parent)
{
    connect(this, &Vpnservice::vlessurlChanged, this, &Vpnservice::generatexrayconfig);
}

void Vpnservice::setvlessurl(const QString &vlessurl)
{
    if (vlessurl == m_vlessurl)
        return;
    m_vlessurl = vlessurl;
    emit vlessurlChanged();
}

QString Vpnservice::qetxraypath()
{
    QString binpath = QCoreApplication::applicationDirPath() + QDir::separator() + "bin" + QDir::separator();

    #if defined(Q_OS_WIN)
        binpath += "windows/xray.exe";
    #elif defined(Q_OS_MAC)
        binpath += "macos/xray";
    #elif defined(Q_OS_LINUX)
       binpath += "linux/xray";
    #else
        binpath += "";
    #endif
    qDebug() << binpath;
    return binpath;
}

void Vpnservice::generatexrayconfig()
{

    qDebug() << m_vlessurl;
    QUrl url(m_vlessurl);
    QString uuid = url.userName();
    QString host = url.host();
    int port = url.port();
    QUrlQuery query(url);

    QString flow       = query.queryItemValue("flow");
    QString encryption = query.queryItemValue("encryption");
    QString security   = query.queryItemValue("security");
    QString sni        = query.queryItemValue("sni");
    QString pbk        = query.queryItemValue("pbk");
    QString sid        = query.queryItemValue("sid");
    QString fp         = query.queryItemValue("fp");

    QJsonObject log;
    log["loglevel"] = "warning";

    QJsonObject inboundSettings;
    inboundSettings["udp"] = true;

    QJsonObject inbound;
    inbound["listen"] = "127.0.0.1";
    inbound["port"] = 1080;
    inbound["protocol"] = "socks";
    inbound["settings"] = inboundSettings;

    QJsonArray inbounds;
    inbounds.append(inbound);

    QJsonObject user;
    user["id"] = uuid;
    if (!flow.isEmpty()) user["flow"] = flow;
    if (!encryption.isEmpty()) user["encryption"] = encryption;

    QJsonArray users;
    users.append(user);

    QJsonObject vnext;
    vnext["address"] = host;
    vnext["port"] = port;
    vnext["users"] = users;

    QJsonArray vnextArray;
    vnextArray.append(vnext);

    QJsonObject outSettings;
    outSettings["vnext"] = vnextArray;

    QJsonObject realitySettings;
    realitySettings["serverName"] = sni;
    realitySettings["publicKey"] = pbk;
    realitySettings["shortId"] = sid;
    realitySettings["fingerprint"] = fp;

    QJsonObject streamSettings;
    streamSettings["network"] = "tcp";
    streamSettings["security"] = security;
    streamSettings["realitySettings"] = realitySettings;

    QJsonObject outbound;
    outbound["protocol"] = "vless";
    outbound["settings"] = outSettings;
    outbound["streamSettings"] = streamSettings;

    QJsonArray outbounds;
    outbounds.append(outbound);

    QJsonObject config;
    config["log"] = log;
    config["inbounds"] = inbounds;
    config["outbounds"] = outbounds;

    m_config = QCoreApplication::applicationDirPath() + QDir::separator()
                      + "xray_temp_config.json";

    QFile file(m_config);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи:" << file.errorString();
        return;
    }

    QJsonDocument doc(config);

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    file.write(jsonData);
    file.close();

    process = new QProcess(this);
    connect(process, &QProcess::stateChanged, this, &Vpnservice::stas);
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput(); 
        QString outputString = QString::fromUtf8(output);      
        qDebug() << "Output:" << outputString;
    });

    connect(process, &QProcess::readyReadStandardError, this, [=]() {
    });


    qDebug() << m_config;
    process->start("/Users/maaoki/Downloads/Xray-macos-arm64-v8a2/xray", QStringList() << "-c" << m_config);
}
