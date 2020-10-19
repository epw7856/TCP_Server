#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

struct ServerSettings
{
    unsigned Port = 0U;
    unsigned TransmissionPeriodicity = 0U;
    bool isLittleEndian = true;
};

#endif // SERVERSETTINGS_H
