/*
  Example: EEPROMManagerDemo
  Demonstrasi penggunaan library EEPROMManager
*/

#include <EEPROMManager.h>

EEPROMManager eepromManager(true);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Tunggu sampai Serial siap (untuk board dengan USB native)
  }
  
  // Inisialisasi EEPROM dengan ukuran 512 byte
  eepromManager.begin(512);

  // Menulis string ke EEPROM pada alamat 10 dan menyimpan panjang string di alamat 0
  String writeResult = eepromManager.writeString("Hello, EEPROM!", 10, 0);
  Serial.println(writeResult);

  // Membaca string dari EEPROM
  String readResult = eepromManager.readString(10, 0);
  Serial.print("Read from EEPROM: ");
  Serial.println(readResult);
}

void loop() {
  // Tidak ada aksi di loop
}