#ifndef NoDelayButton_H
#define NoDelayButton_H

#include <Arduino.h>

class NoDelayButton
{
public:
  // Mode digital
  NoDelayButton(uint8_t pins[], char buttonChars[], uint8_t buttonCount, uint8_t expectedButtonCount);

  // Mode analog
  NoDelayButton(uint8_t analogPin, char buttonChars[], uint8_t buttonCount, uint16_t _bits);

  bool begin();
  uint8_t scanValue();       // digital atau analog, otomatis
  uint8_t scanValueRepeat(); // untuk scan tanpa kunci
  void clear();              // reset kunci
  void lockButton();         // mengunci agar tidak double input
  String scanString();       // baca string multi tombol digital
  char convertChar(int8_t index);

private:
  // Umum
  char buttonChars[10];
  uint8_t buttonCount;
  uint8_t expectedButtonCount;
  bool isLocked = false;
  uint8_t pressedButtonIndex = 255;

  // Digital
  uint8_t buttonPins[10];

  // Analog
  bool useAnalog = false;
  uint8_t analogPin;
  uint16_t bits;

  uint8_t scanDigital();
  uint8_t scanAnalog();
};

#endif