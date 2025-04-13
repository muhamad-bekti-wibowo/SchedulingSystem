#ifndef TIMECONVERTER_H
#define TIMECONVERTER_H

#include <Arduino.h>

#define SEVENTY_YEARS 2208988800UL
#define SECONDS_IN_DAY 86400L
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define LEAP_YEARS(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))

class TimeConverter {
public:
  TimeConverter();  // Konstruktor
  
  int getDayOfWeek(uint64_t timestamp);
  int getHours(uint64_t timestamp);
  int getMinutes(uint64_t timestamp);
  int getSeconds(uint64_t timestamp);
  
  String formatTime(uint64_t timestamp);
  String formatDate(uint64_t timestamp);

  uint64_t convertToEpoch(int year, int month, int day, int hour, int minute, int second);
  void convertFromEpoch(uint64_t epochTime, int &year, int &month, int &day, int &days, int &hour, int &minute, int &second);

};

#endif