#include "NoDelayButton.h"

// Konstruktor digital
NoDelayButton::NoDelayButton(uint8_t pins[], char _buttonChars[], uint8_t _buttonCount, uint8_t _expectedButtonCount)
    : buttonCount(_buttonCount), expectedButtonCount(_expectedButtonCount), useAnalog(false)
{
  for (uint8_t i = 0; i < buttonCount; i++)
  {
    buttonPins[i] = pins[i];
    buttonChars[i] = _buttonChars[i];
  }
}

// Konstruktor analog
NoDelayButton::NoDelayButton(uint8_t _analogPin, char _buttonChars[], uint8_t _buttonCount, uint16_t _bits)
    : analogPin(_analogPin), buttonCount(_buttonCount), expectedButtonCount(_buttonCount + 1), useAnalog(true), bits(_bits)
{
  for (uint8_t i = 0; i < buttonCount; i++)
  {
    buttonChars[i] = _buttonChars[i];
  }
}

uint8_t NoDelayButton::scanValue()
{
  return useAnalog ? scanAnalog() : scanDigital();
}

uint8_t NoDelayButton::scanDigital()
{
  uint8_t activeCount = 0;
  for (uint8_t i = 0; i < buttonCount; i++)
  {
    if (!digitalRead(buttonPins[i]))
    {
      if (!isLocked)
        pressedButtonIndex = i;
    }
    else
    {
      activeCount++;
    }
  }

  if (activeCount == buttonCount)
    pressedButtonIndex = 255;
  if (pressedButtonIndex == 255)
    isLocked = false;
  else if (isLocked)
    pressedButtonIndex = 254;
  if (pressedButtonIndex > 0 && pressedButtonIndex < 255 && !isLocked)
    isLocked = true;

  return pressedButtonIndex;
}

uint8_t NoDelayButton::scanAnalog()
{
  int analogValue = analogRead(analogPin);
  int rangeSize = bits / buttonCount;
  int index = analogValue / rangeSize;

  if (index >= 0 && index < buttonCount)
  {
    if (!isLocked)
    {
      pressedButtonIndex = index;
      isLocked = true;
    }
    else
    {
      pressedButtonIndex = 254;
    }
  }
  else
  {
    pressedButtonIndex = 255;
  }

  if (pressedButtonIndex == 255)
    isLocked = false;
  return pressedButtonIndex;
}

uint8_t NoDelayButton::scanValueRepeat()
{
  clear();
  return scanValue();
}

String NoDelayButton::scanString()
{
  String pinPressed = "";
  uint8_t activeCount = 0;

  if (useAnalog)
    return ""; // scanString hanya untuk digital

  for (uint8_t i = 0; i < buttonCount; i++)
  {
    if (!digitalRead(buttonPins[i]))
    {
      pinPressed += char(buttonChars[i]);
    }
    else
    {
      activeCount++;
    }
  }

  if (pinPressed.length() == expectedButtonCount && activeCount != 1)
  {
    return pinPressed;
  }

  return "";
}

void NoDelayButton::clear()
{
  isLocked = false;
}

void NoDelayButton::lockButton()
{
  isLocked = true;
}

bool NoDelayButton::begin()
{
  return scanValue() == 255;
}

char NoDelayButton::convertChar(int8_t index)
{
  if (index >= 0 && index < buttonCount)
    return buttonChars[index];
  return '?';
}