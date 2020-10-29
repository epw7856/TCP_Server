#include "datautilities.h"
#include <QIntValidator>
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

bool SystemDataSource::convertOutboundData(std::vector<unsigned>& outboundData, std::string rawData)
{
    QStringList dataList = QString::fromStdString(rawData).split("\n");

    for(int i = 0; i < dataList.size(); ++i)
    {
        unsigned value = 0U;
        DataType type = outboundDataTypes[i];
        switch (type)
        {
            case DataType::Unsigned:
                {
                    if(validateInt(dataList[i]))
                    {
                        value = dataList[i].toUInt();
                    }
                    else
                    {
                        return false;
                    }
                }
                break;

            case DataType::Float:
                {
                    if(validateFloat(dataList[i]))
                    {
                        value = floatToUnsigned(dataList[i].toFloat());
                    }
                    else
                    {
                        return false;
                    }
                }
                break;

            case DataType::Int:
                {
                    if(validateInt(dataList[i]))
                    {
                        value = intToUnsigned(dataList[i].toInt());
                    }
                    else
                    {
                        return false;
                    }
                }
                break;

            case DataType::Unknown:
                return false;
        }

        outboundData.push_back(value);
    }
    return true;
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
        emit sendErrorMessage("System Configuration Error", "Unable to create new System Configuration File!");
        return;
    }
    configFile.close();

    QJsonValue empty;

    obj.insert("DataToClient", empty);
    obj.insert("DataFromClient", empty);

    dataTypesValid = false;
}

void SystemDataSource::loadConfigFile()
{
    if(!configFile.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        emit sendErrorMessage("System Configuration Error", "Unable to load System Configuration File!");
        return;
    }

    QByteArray rawData = configFile.readAll();
    configFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    obj = doc.object();

    if(!parseDataTypes(JsonDataSection::DataToClient) || !parseDataTypes(JsonDataSection::DataFromClient))
    {
        dataTypesValid = false;
        emit sendErrorMessage("System Configuration Error", "Unknown data type found in System Configuration!");
    }
    else
    {
        dataTypesValid = true;
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

bool SystemDataSource::validateUnsigned(QString& value) const
{
    bool status;
    value.toUInt(&status, 10);
    return status;
}

bool SystemDataSource::validateFloat(QString& value) const
{
    QDoubleValidator val(0.000000001, 100000000.0000, 9);

    int pos = 0;
    return (val.validate(value, pos) != QValidator::Invalid);
}

bool SystemDataSource::validateInt(QString& value) const
{
    QIntValidator val(-2147483648, 2147483647);
    int pos = 0;
    return (val.validate(value, pos) != QValidator::Invalid);
}
