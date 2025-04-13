#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

class WiFiScanner {
  public:
    // Constructor with optional debug flag
    WiFiScanner(bool debug = false);
    
    // Scan for networks and check if specific SSID is available
    bool isSSIDAvailable(const String &SSID);
    
    // Perform full network scan and print results
    void fullScan();
    
    // Get number of available networks from last scan
    int getNetworkCount();
    
    // Get network info by index from last scan
    bool getNetworkInfo(int index, String &ssid, int32_t &rssi, 
                       uint8_t &encryptionType, uint8_t *bssid, 
                       int32_t &channel, bool &hidden);
    
    // Set debug mode
    void setDebug(bool debug);
    
    // Get last scan result code
    int getLastScanResult();
    
  private:
    bool _debug;
    int _lastScanResult;
    int _networkCount;
    
    void printNetworkInfo(int index, const String &ssid, uint8_t encryptionType, 
                         int32_t rssi, uint8_t *bssid, int32_t channel, 
                         bool hidden);
    String getEncryptionTypeStr(uint8_t encryptionType);
    String getPhyMode(const bss_info *bssInfo);
};

#endif