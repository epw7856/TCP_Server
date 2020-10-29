#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include "systemdatasource.h"

SystemDataSource::SystemDataSource()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    configFile.setFileName(dir.path() + "/SystemConfiguration.json");

    (!dir.exists() || !configFile.exists()) ? createConfigFile() :
                                              loadConfigFile();
}

bool SystemDataSource::setConfigurationDirectory()
{
    configPath = QString("%1%2").arg(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).arg("/.TCPServer/");
    QDir dir(configPath);
    return (dir.exists()) ? true : dir.mkpath(configPath);
}

void SystemDataSource::createConfigFile()
{
    QFileInfo info(configFile);
    QDir dir;
    dir.mkpath(info.absolutePath());

    if(!configFile.open(QIODevice::WriteOnly))
    {
        emit sendErrorMessage("Unable to create new System Configuration File!");
    }
    configFile.close();

    QJsonValue empty;

    obj.insert("DataToClient", empty);
    obj.insert("DataFromClient", empty);
}

void SystemDataSource::loadConfigFile()
{
    if(!configFile.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        emit sendErrorMessage("Unable to load System Configuration File!");
    }

    QByteArray rawData = configFile.readAll();
    configFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    obj = doc.object();

    if(!parseDataTypes(JsonDataSection::DataToClient) || !parseDataTypes(JsonDataSection::DataFromClient))
    {
        emit sendErrorMessage("Unknown data type found in System Configuration!");
    }
}

bool SystemDataSource::parseDataTypes(JsonDataSection section)
{

    QJsonValue jsonOutboundDataTypes;

    if(section == JsonDataSection::DataToClient)
    {
        jsonOutboundDataTypes = obj.value("DataToClient");
    }
    else if(section == JsonDataSection::DataFromClient)
    {
        jsonOutboundDataTypes = obj.value("DataFromClient");
    }

    QJsonArray array = jsonOutboundDataTypes.toArray();
    for (const QJsonValue item : array)
    {
        DataType type = stringToDataType(item.toString());
        if(type == DataType::Unknown)
        {
            return false;
        }
        else
        {
            if(section == JsonDataSection::DataToClient)
            {
                outboundDataTypes.push_back(type);
            }
            else if(section == JsonDataSection::DataFromClient)
            {
                inboundDataTypes.push_back(type);
            }
        }
    }
    return true;
}

DataType SystemDataSource::stringToDataType(const QString& value) const
{
    if(value.toLower() == "unsigned")
    {
        return DataType::Unsigned;
    }
    else if(value.toLower() == "float")
    {
        return DataType::Float;
    }
    else if(value.toLower() == "int")
    {
        return DataType::Int;
    }
    else
    {
        return DataType::Unknown;
    }
}
