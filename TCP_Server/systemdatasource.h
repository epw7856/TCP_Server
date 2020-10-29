#ifndef SYSTEMDATASOURCE_H
#define SYSTEMDATASOURCE_H

#include "datatype.h"
#include <QDir>
#include <QJsonObject>
#include <QString>

class SystemDataSource : public QObject
{
    Q_OBJECT
public:
    SystemDataSource();

signals:
    void sendErrorMessage(std::string msg);

private:
    QString configPath = "";
    QFile configFile;
    QJsonObject obj;
    std::vector<DataType> inboundDataTypes = {};
    std::vector<DataType> outboundDataTypes = {};
    enum class JsonDataSection{ DataToClient, DataFromClient };

    bool setConfigurationDirectory();
    void createConfigFile();
    void loadConfigFile();
    bool parseDataTypes(JsonDataSection section);
    DataType stringToDataType(const QString& value) const;
};

#endif // SYSTEMDATASOURCE_H
