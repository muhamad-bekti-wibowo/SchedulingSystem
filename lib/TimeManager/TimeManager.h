#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>
#include "TimeConverter.h"

class TimeManager
{
public:
    // Constructor with optional debug
    TimeManager(const char *ntpServer = "pool.ntp.org",
                long timeOffset = 25200,
                bool debug = false);

    // Initialize time client
    void begin();

    // Update time from NTP server
    bool update();

    // Get current epoch time
    uint64_t getEpochTime();

    // Get formatted date/time string
    String getFormattedTime();
    String getFormattedDate();

    // Get individual time components
    int getYear();
    int getMonth();
    int getDay();
    int getHours();
    int getMinutes();
    int getSeconds();

    // Time conversion utilities
    static uint64_t timeToUInt64(time_t timeValue);
    static time_t uInt64ToTime(uint64_t timeValue);

    // Set debug mode
    void setDebug(bool debug);

    // Manual time adjustment
    void setTimeOffset(long offset);
    void adjustTime(long seconds);

private:
    TimeConverter _timeConv;
    WiFiUDP _ntpUDP;
    NTPClient _timeClient;
    const char *_ntpServer;
    long _timeOffset;
    bool _debug;
    bool _timeSynced;
    uint64_t _millisOffset;
    uint64_t _epochTimeOffset;

    void _logDebug(const String &message);
    void _logError(const String &message);
};

#endif