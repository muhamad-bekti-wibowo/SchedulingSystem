#include "TimeConverter.h"

TimeConverter::TimeConverter() {}

int TimeConverter::getDayOfWeek(uint64_t  timestamp) {
  return (((timestamp / SECONDS_IN_DAY) + 4) % 7); // 0 is Sunday
}

int TimeConverter::getHours(uint64_t  timestamp) {
  return (timestamp % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
}

int TimeConverter::getMinutes(uint64_t  timestamp) {
  return (timestamp % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
}

int TimeConverter::getSeconds(uint64_t  timestamp) {
  return timestamp % SECONDS_IN_MINUTE;
}

String TimeConverter::formatTime(uint64_t  timestamp) {
  int hours = getHours(timestamp);
  int minutes = getMinutes(timestamp);
  int seconds = getSeconds(timestamp);

  return (hours < 10 ? "0" + String(hours) : String(hours)) + ":" +
         (minutes < 10 ? "0" + String(minutes) : String(minutes)) + ":" +
         (seconds < 10 ? "0" + String(seconds) : String(seconds));
}

String TimeConverter::formatDate(uint64_t  timestamp) {
  unsigned long days = 0, year = 1970;
  unsigned long rawDays = timestamp / SECONDS_IN_DAY;
  uint8_t month;
  static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  while (days + (LEAP_YEARS(year) ? 366 : 365) <= rawDays) {
    days += (LEAP_YEARS(year) ? 366 : 365);
    year++;
  }

  rawDays -= days;
  days = 0;

  for (month = 0; month < 12; month++) {
    uint8_t monthLength = (month == 1) ? (LEAP_YEARS(year) ? 29 : 28) : monthDays[month];
    if (rawDays < monthLength) break;
    rawDays -= monthLength;
  }

  return String(year) + "-" +
         (month + 1 < 10 ? "0" + String(month + 1) : String(month + 1)) + "-" +
         (rawDays + 1 < 10 ? "0" + String(rawDays + 1) : String(rawDays + 1)) +
         " " + formatTime(timestamp);
}

uint64_t TimeConverter::convertToEpoch(int year, int month, int day, int hour, int minute, int second) {
    uint64_t epochTime = 0;
    int days = 0;
    static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
    for (int y = 1970; y < year; y++) {
      days += (LEAP_YEARS(y) ? 366 : 365);
    }
  
    for (int m = 0; m < month - 1; m++) {
      days += (m == 1 ? (LEAP_YEARS(year) ? 29 : 28) : monthDays[m]);
    }
  
    days += day - 1;
    epochTime = (uint64_t)days * SECONDS_IN_DAY + hour * SECONDS_IN_HOUR + minute * SECONDS_IN_MINUTE + second;
    
    return epochTime;
  }
  
  void TimeConverter::convertFromEpoch(uint64_t epochTime, int &year, int &month, int &day, int &days, int &hour, int &minute, int &second) {
    days = epochTime / SECONDS_IN_DAY;
    hour = (epochTime % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
    minute = (epochTime % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
    second = epochTime % SECONDS_IN_MINUTE;
  
    year = 1970;
    static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
    while (days >= (LEAP_YEARS(year) ? 366 : 365)) {
      days -= (LEAP_YEARS(year) ? 366 : 365);
      year++;
    }
  
    month = 0;
    for (int m = 0; m < 12; m++) {
      int monthLength = (m == 1 ? (LEAP_YEARS(year) ? 29 : 28) : monthDays[m]);
      if (days < monthLength) {
        month = m + 1;
        day = days + 1;
        break;
      }
      days -= monthLength;
    }
  }