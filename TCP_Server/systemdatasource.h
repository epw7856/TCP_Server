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
    bool areDataTypesValid() const;
    bool convertOutboundData(std::vector<unsigned>& outboundData, std::string rawData);
    bool convertInboundData(std::vector<unsigned>& inboundData, std::string& rawData);

signals:
    void sendErrorMessage(std::string title, std::string msg);

private:
    QString configPath = "";
    QFile configFile;
    QJsonObject obj;
    std::vector<DataType> inboundDataTypes = {};
    std::vector<DataType> outboundDataTypes = {};
    bool dataTypesValid = false;
    enum class JsonDataSection{ DataToClient, DataFromClient };

    bool setConfigurationDirectory();
    void createConfigFile();
    void loadConfigFile();
    bool parseDataTypes(JsonDataSection section);
    DataType stringToDataType(const QString& value) const;
    bool validateUnsigned(QString& value) const;
    bool validateFloat(QString& value) const;
    bool validateInt(QString& value) const;
};

inline bool SystemDataSource::areDataTypesValid() const
{
    return dataTypesValid;
}

#endif // SYSTEMDATASOURCE_H
