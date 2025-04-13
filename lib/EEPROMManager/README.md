# EEPROMManager Library

EEPROMManager adalah library untuk Arduino yang menyediakan manajemen EEPROM dengan kemampuan debugging. Library ini memudahkan Anda untuk menulis, membaca, dan menghapus data EEPROM dengan tambahan log debugging melalui Serial.

## Fitur

- Menulis dan membaca string ke/dari EEPROM.
- Menulis dan membaca byte ke/dari EEPROM.
- Penghapusan (erase) bagian EEPROM.
- Debugging dengan log ke Serial monitor.

## Instalasi

1. Clone atau download repository dari GitHub.
2. Salin folder `EEPROMManager` ke direktori libraries Arduino (misalnya: `Documents/Arduino/libraries/`).
3. Restart Arduino IDE jika sedang berjalan.

## Cara Penggunaan

Tambahkan library ini ke sketch Arduino Anda:
```cpp
#include <EEPROMManager.h>

EEPROMManager eepromManager(true);

void setup() {
  Serial.begin(115200);
  // Inisialisasi EEPROM dengan ukuran tertentu (misalnya 512 byte)
  eepromManager.begin(512);

  // Menulis string ke EEPROM
  String result = eepromManager.writeString("Hello, EEPROM!", 10, 0);
  Serial.println(result);

  // Membaca string dari EEPROM
  String data = eepromManager.readString(10, 0);
  Serial.println(data);
}

void loop() {
  // Kode program lainnya
}