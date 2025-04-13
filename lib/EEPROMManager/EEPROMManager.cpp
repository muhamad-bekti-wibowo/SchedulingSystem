/**
 * EEPROMManager.cpp - Implementation of Enhanced EEPROM manager
 */
#include "EEPROMManager.h"

// Private logging methods
void EEPROMManager::_logDebug(const String &message)
{
    // Cetak hanya jika debug aktif DAN Serial telah diinisialisasi
    if (_debug && Serial)
    {
        Serial.print("[EEPROM DEBUG] ");
        Serial.println(message);
    }
}

void EEPROMManager::_logError(const String &message)
{
    // Selalu coba cetak error jika Serial telah diinisialisasi
    if (Serial)
    {
        Serial.print("[EEPROM ERROR] ");
        Serial.println(message);
    }
}
void EEPROMManager::begin(uint16_t size)
{
    _size = size;
    EEPROM.begin(size);
    if (_debug)
    {
        _logDebug("Initialized with size: " + String(_size));
    }
}
// Constructor
EEPROMManager::EEPROMManager(bool debug)
{

    _debug = debug;
}

// Set debug mode
void EEPROMManager::setDebug(bool enable)
{
    _debug = enable;

    if (_debug && !Serial)
    {
        Serial.begin(115200);
        delay(100); // Allow time for Serial to initialize
        _logDebug("Debug mode enabled");
    }
    else if (_debug)
    {
        _logDebug("Debug mode enabled");
    }
}

// Write string to EEPROM
String EEPROMManager::writeString(String data, uint16_t start, uint16_t lengthAddress)
{
    _logDebug("Writing string: \"" + data + "\"");
    _logDebug("Start address: " + String(start));
    _logDebug("Length stored at: " + String(lengthAddress));

    // Validate parameters
    if (start + data.length() >= _size)
    {
        _logError("String exceeds EEPROM size");
        return "Error: Out of bounds";
    }

    if (lengthAddress >= _size)
    {
        _logError("Length address out of bounds");
        return "Error: Length address invalid";
    }

    uint16_t dataSize = data.length();

    // Write length to specified address
    EEPROM.write(lengthAddress, dataSize);

    // Write string data
    for (uint16_t i = 0; i < dataSize; i++)
    {
        EEPROM.write(start + i, data[i]);
    }

    // Add null terminator
    EEPROM.write(start + dataSize, '\0');

    // Commit changes
    bool success = EEPROM.commit();

    if (success)
    {
        _logDebug("Write successful");
        return "Success";
    }
    else
    {
        _logError("Failed to commit changes");
        return "Failed to commit";
    }
}

// Read string from EEPROM
String EEPROMManager::readString(uint16_t start, uint16_t lengthAddress, uint16_t length)
{
    // Get length either from storage or parameter
    uint16_t dataLength;

    if (lengthAddress > 0)
    {
        if (lengthAddress >= _size)
        {
            _logError("Length address out of bounds");
            return "Error: Length address invalid";
        }
        dataLength = EEPROM.read(lengthAddress);
    }
    else
    {
        dataLength = length;
    }

    _logDebug("Reading string from address: " + String(start));
    _logDebug("String length: " + String(dataLength));

    // Validate parameters
    if (start + dataLength >= _size)
    {
        _logError("Read operation exceeds EEPROM size");
        return "Error: Out of bounds";
    }

    char data[dataLength + 1];

    // Read character by character
    for (uint16_t i = 0; i < dataLength; i++)
    {
        data[i] = EEPROM.read(start + i);
    }

    // Add null terminator
    data[dataLength] = '\0';

    String result = String(data);

    _logDebug("Read string: \"" + result + "\"");

    return result;
}

// Read byte from EEPROM
uint8_t EEPROMManager::readByte(uint16_t address)
{
    if (address >= _size)
    {
        _logError("Read address out of bounds");
        return 0;
    }

    uint8_t value = EEPROM.read(address);

    _logDebug("Read byte at address " + String(address) + ": " + String(value));

    return value;
}

// Write byte to EEPROM
String EEPROMManager::writeByte(uint16_t address, uint8_t value)
{
    if (address >= _size)
    {
        _logError("Write address out of bounds");
        return "Error: Address out of bounds";
    }

    _logDebug("Writing byte: " + String(value) + " to address: " + String(address));

    EEPROM.write(address, value);
    bool success = EEPROM.commit();

    if (success)
    {
        _logDebug("Byte written successfully");
        return "Success";
    }
    else
    {
        _logError("Failed to commit changes");
        return "Failed to commit";
    }
}

// Erase section of EEPROM
String EEPROMManager::eraseSection(uint16_t start, uint16_t length)
{
    _logDebug("Erasing section from: " + String(start) + ", length: " + String(length));

    // Validate parameters
    if (start + length >= _size)
    {
        _logError("Erase operation exceeds EEPROM size");
        return "Error: Out of bounds";
    }

    // Write zeros to each address
    for (uint16_t i = 0; i < length; i++)
    {
        EEPROM.write(start + i, 0);
    }

    bool success = EEPROM.commit();

    if (success)
    {
        _logDebug("Section erased successfully");
        return "Success";
    }
    else
    {
        _logError("Failed to commit changes");
        return "Failed to commit";
    }
}