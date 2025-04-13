#include "WiFiScanner.h"

WiFiScanner::WiFiScanner(bool debug) : _debug(debug), _lastScanResult(0), _networkCount(0) {
}

bool WiFiScanner::isSSIDAvailable(const String &SSID) {
    if (_debug) {
        Serial.println(F("[WiFiScanner] Starting WiFi scan..."));
    }
    
    _lastScanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
    _networkCount = _lastScanResult;
    
    if (_lastScanResult == 0) {
        if (_debug) {
            Serial.println(F("[WiFiScanner] No networks found"));
        }
        return false;
    } 
    else if (_lastScanResult > 0) {
        if (_debug) {
            Serial.printf(PSTR("[WiFiScanner] %d networks found\n"), _lastScanResult);
        }
        
        for (int8_t i = 0; i < _lastScanResult; i++) {
            String ssid;
            int32_t rssi;
            uint8_t encryptionType;
            uint8_t *bssid;
            int32_t channel;
            bool hidden;
            
            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
            
            if (_debug) {
                printNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
            }
            
            if (ssid == SSID) {
                if (_debug) {
                    Serial.printf(PSTR("[WiFiScanner] Found target SSID: %s\n"), SSID.c_str());
                }
                return true;
            }
            yield();
        }
    }
    else {
        if (_debug) {
            Serial.printf(PSTR("[WiFiScanner] WiFi scan error %d\n"), _lastScanResult);
        }
    }
    return false;
}

void WiFiScanner::fullScan() {
    _lastScanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
    _networkCount = _lastScanResult;
    
    if (_lastScanResult == 0) {
        Serial.println(F("[WiFiScanner] No networks found"));
    } 
    else if (_lastScanResult > 0) {
        Serial.printf(PSTR("[WiFiScanner] %d networks found:\n"), _lastScanResult);
        
        for (int8_t i = 0; i < _lastScanResult; i++) {
            String ssid;
            int32_t rssi;
            uint8_t encryptionType;
            uint8_t *bssid;
            int32_t channel;
            bool hidden;
            
            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
            printNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
            yield();
        }
    }
    else {
        Serial.printf(PSTR("[WiFiScanner] WiFi scan error %d\n"), _lastScanResult);
    }
}

int WiFiScanner::getNetworkCount() {
    return _networkCount;
}

bool WiFiScanner::getNetworkInfo(int index, String &ssid, int32_t &rssi, 
                               uint8_t &encryptionType, uint8_t *bssid, 
                               int32_t &channel, bool &hidden) {
    if (index >= 0 && index < _networkCount) {
        WiFi.getNetworkInfo(index, ssid, encryptionType, rssi, bssid, channel, hidden);
        return true;
    }
    return false;
}

void WiFiScanner::setDebug(bool debug) {
    _debug = debug;
}

int WiFiScanner::getLastScanResult() {
    return _lastScanResult;
}

void WiFiScanner::printNetworkInfo(int index, const String &ssid, uint8_t encryptionType, 
                                 int32_t rssi, uint8_t *bssid, int32_t channel, 
                                 bool hidden) {
    const bss_info *bssInfo = WiFi.getScanInfoByIndex(index);
    
    Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %s %c %-11s %s\n"), 
                 index, 
                 channel, 
                 bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], 
                 rssi, 
                 getEncryptionTypeStr(encryptionType).c_str(),
                 hidden ? 'H' : 'V', 
                 getPhyMode(bssInfo).c_str(), 
                 ssid.c_str());
}

String WiFiScanner::getEncryptionTypeStr(uint8_t encryptionType) {
    switch(encryptionType) {
        case ENC_TYPE_NONE: return "OPEN";
        case ENC_TYPE_WEP: return "WEP";
        case ENC_TYPE_TKIP: return "WPA";
        case ENC_TYPE_CCMP: return "WPA2";
        case ENC_TYPE_AUTO: return "AUTO";
        default: return "UNKN";
    }
}

String WiFiScanner::getPhyMode(const bss_info *bssInfo) {
    String phyMode = "802.11";
    String slash;
    
    if (bssInfo) {
        if (bssInfo->phy_11b) {
            phyMode += 'b';
            slash = '/';
        }
        if (bssInfo->phy_11g) {
            phyMode += slash + 'g';
            slash = '/';
        }
        if (bssInfo->phy_11n) {
            phyMode += slash + 'n';
        }
        #ifdef ESP32
        if (bssInfo->phy_lr) {
            phyMode += slash + "LR";
        }
        if (bssInfo->phy_11ax) {
            phyMode += slash + "ax";
        }
        #endif
    }
    
    return phyMode;
}