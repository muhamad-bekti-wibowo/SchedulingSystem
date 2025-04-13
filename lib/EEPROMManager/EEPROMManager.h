/**
 * EEPROMManager.h - Enhanced EEPROM manager with debugging capabilities
 */
#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

class EEPROMManager {
private:
  bool _debug;
  uint16_t _size;
  
  // Private logging methods
  void _logDebug(const String &message);
  void _logError(const String &message);

public:
  /**
   * Constructor for the EEPROMManager class
   * @param size Size of EEPROM to initialize
   * @param debug Enable debug messages (default: false)
   */
  EEPROMManager(bool debug = false);
  void begin(uint16_t size);
  /**
   * Enable or disable debug messages
   * @param enable True to enable, false to disable
   */
  void setDebug(bool enable);
  
  /**
   * Write a string to EEPROM
   * @param data String to write
   * @param start Starting address
   * @param lengthAddress Address to store the string length
   * @return Result message
   */
  String writeString(String data, uint16_t start, uint16_t lengthAddress);
  
  /**
   * Read a string from EEPROM
   * @param start Starting address
   * @param lengthAddress Address where string length is stored (0 to use length parameter)
   * @param length Maximum length to read (used if lengthAddress is 0)
   * @return String read from EEPROM
   */
  String readString(uint16_t start, uint16_t lengthAddress = 0, uint16_t length = 0);
  
  /**
   * Read a byte value from EEPROM
   * @param address Address to read from
   * @return Byte value
   */
  uint8_t readByte(uint16_t address);
  
  /**
   * Write a byte value to EEPROM
   * @param address Address to write to
   * @param value Byte value to write
   * @return Result message
   */
  String writeByte(uint16_t address, uint8_t value);
  
  /**
   * Erase a section of EEPROM by writing zeros
   * @param start Starting address
   * @param length Number of bytes to erase
   * @return Result message
   */
  String eraseSection(uint16_t start, uint16_t length);
};

#endif // EEPROM_MANAGER_H