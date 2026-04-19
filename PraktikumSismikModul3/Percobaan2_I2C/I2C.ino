#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C: Alamat 0x27 atau 0x20, ukuran 16 kolom x 2 baris
// Di Tinkercad biasanya menggunakan alamat 0x27 atau 32
LiquidCrystal_I2C lcd(0x27, 16, 2); 
 
const int pinPot = A0;

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Tampilan awal
  lcd.setCursor(0, 0);
  lcd.print("System Ready...");
  delay(1000);
  lcd.clear();
}

void loop() {
  // 1. Baca nilai analog dari potensiometer (0-1023)
  int nilai = analogRead(pinPot);
  
  // 2. Mapping nilai ke panjang bar (0-16 karakter)
  int panjangBar = map(nilai, 0, 1023, 0, 16);
  
  // 3. Tampilkan ke Serial Monitor
  Serial.print("Nilai ADC : ");
  Serial.println(nilai);
  
  // 4. Baris 1: Menampilkan teks dan nilai angka
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.print(nilai);
  lcd.print("    "); // Menghapus sisa angka sebelumnya agar tidak menumpuk
  
  // 5. Baris 2: Menampilkan Bar Progress
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Karakter blok penuh
    } else {
      lcd.print(" "); // Karakter kosong
    }
  }
  
  delay(100); // Delay singkat agar pergerakan bar halus
}