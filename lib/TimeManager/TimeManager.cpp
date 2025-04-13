#include "TimeManager.h"

TimeManager::TimeManager(const char *ntpServer, long timeOffset, bool debug)
    : _timeClient(_ntpUDP, ntpServer, timeOffset, 60000),
      _ntpServer(ntpServer),
      _debug(debug),
      _timeSynced(false),
      _millisOffset(0),
      _epochTimeOffset(0)
{
}

void TimeManager::begin()
{
    _timeClient.begin();
    _logDebug("Time client initialized with server: " + String(_ntpServer));
}

bool TimeManager::update()
{
    if (!_timeClient.update())
    {
        _logError("Failed to update time from NTP server");
        return false;
    }

    _epochTimeOffset = _timeClient.getEpochTime();
    _timeSynced = true;
    _logDebug("Time successfully updated: " + _timeConv.formatDate(_epochTimeOffset));
    return true;
}

uint64_t TimeManager::getEpochTime()
{
    if (!_timeSynced)
    {
        _logDebug("Warning: Time not yet synced, returning 0");
        return 0;
    }
    return _epochTimeOffset + (millis() - _millisOffset) / 1000;
}

String TimeManager::getFormattedTime()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    return String(buffer);
}

String TimeManager::getFormattedDate()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    return String(buffer);
}

int TimeManager::getYear()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_year + 1900;
}

int TimeManager::getMonth()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_mon + 1;
}

int TimeManager::getDay()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_mday;
}

int TimeManager::getHours()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_hour;
}

int TimeManager::getMinutes()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_min;
}

int TimeManager::getSeconds()
{
    time_t now = uInt64ToTime(getEpochTime());
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_sec;
}

uint64_t TimeManager::timeToUInt64(time_t timeValue)
{
    return static_cast<uint64_t>(timeValue);
}

time_t TimeManager::uInt64ToTime(uint64_t timeValue)
{
    return static_cast<time_t>(timeValue);
}

void TimeManager::setDebug(bool debug)
{
    _debug = debug;
    _logDebug("Debug mode " + String(debug ? "enabled" : "disabled"));
}

void TimeManager::setTimeOffset(long offset)
{
    _timeClient.setTimeOffset(offset);
    _logDebug("Time offset set to: " + String(offset));
}

void TimeManager::adjustTime(long seconds)
{
    _epochTimeOffset += seconds;
    _logDebug("Time adjusted by: " + String(seconds) + " seconds");
}


void TimeManager::_logDebug(const String &message)
{
    if (_debug)
    {
        Serial.println("[TimeManager] " + message);
    }
}

void TimeManager::_logError(const String &message)
{
    Serial.println("[TimeManager] ERROR: " + message);
}