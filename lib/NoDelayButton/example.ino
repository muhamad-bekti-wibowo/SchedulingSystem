/*
  Example: NoDelayButtonDemo
  Demonstrasi penggunaan library NoDelayButton
*/

#include <NoDelayButton.h>

// Definisikan pin dan karakter untuk tombol
uint8_t pins[] = {2, 3, 4};
char buttonChars[] = {'A', 'B', 'C'};
const uint8_t expectedButtonCount = 3;

// Inisialisasi objek NoDelayButton
NoDelayButton button(pins, buttonChars, 3, expectedButtonCount);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Tunggu sampai Serial siap (jika menggunakan board dengan USB native)
  }
  
  if (button.begin()) {
    Serial.println("NoDelayButton ready!");
  } else {
    Serial.println("NoDelayButton not ready.");
  }
}

void loop() {
  uint8_t value = button.scanValue();
  if (value < 255 && value > 0) {
    Serial.print("Button pressed: ");
    Serial.println(button.convertChar(value - 1));
  }
  
  // Jika Anda ingin mendapatkan kombinasi tombol yang ditekan
  String pressedCombo = button.scanString();
  if (pressedCombo.length() > 0) {
    Serial.print("Buttons pressed: ");
    Serial.println(pressedCombo);
  }
  
  delay(100);
}