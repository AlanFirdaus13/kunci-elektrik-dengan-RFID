#include <SPI.h>
#include <RFID.h>
#include <EEPROM.h>
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);

int data_RFID[4] = { 0, 0, 0, 0 };
bool mode_daftar = false;
bool kondisi_LED = false;
char text;

#define tolak_akses 2  // led
#define verifikasi  5  // led
#define relay_motor 3  // relay
#define notifikasi  6  // buzzer
#define bt_status   4  // button 

#define BAUDRATE 9600

void setup() {
  Serial.begin(BAUDRATE);
  SPI.begin();
  rfid.init();

  pinMode(tolak_akses,OUTPUT);
  pinMode(verifikasi, OUTPUT);
  pinMode(relay_motor,OUTPUT);
  pinMode(notifikasi, OUTPUT);

  digitalWrite(relay_motor, HIGH);
  digitalWrite(tolak_akses, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    text = Serial.read();
    if (text == 'd') {
      mode_daftar = true;
    } 
    else if (text == 'h') {
      for (int i = 0; i <= 3; i++) {
        data_RFID[i] = 0;
      }
      Serial.println(">> RFID Dihapus <<");
      delay(2000);
    }
  }
  pendaftaran();
}

void pendaftaran() {
  if (mode_daftar == true) {                                                                 
    if (data_RFID[0] != 0 && data_RFID[1] != 0 && data_RFID[2] != 0 && data_RFID[3] != 0) {  
      goto gagal_akses;                                                                     
    } 
    else {                                                                                
      Serial.println("Scan Kartu...");  
      led_verifikasi();                                                     
      if (rfid.isCard() && rfid.readCardSerial()) {                                          
        for (int i = 0; i <= 3; i++) {
          data_RFID[i] = rfid.serNum[i];  
        }
        rfid.halt();                        
        Serial.println("Kartu Terdaftar");  
        delay(2000);                        
        mode_daftar = false;                
      }
      delay(1000);
    }
  }

  else if (mode_daftar == false) {
    kontrol_relay();
    if (rfid.isCard() && rfid.readCardSerial()) {
      for (int i = 0; i <= 3; i++) {
        if (rfid.serNum[i] != data_RFID[i]) {
          goto gagal_akses;
        }
      }
      rfid.halt();
      Serial.println("<< Akses Diterima >>");
      delay(1000);
      
      led_status();
    } 
  }

  else {
    gagal_akses:
    Serial.println("Akses Ditolak");
    digitalWrite(relay_motor, HIGH);
    delay(2000);
    mode_daftar = false;
  }
}

void kontrol_relay() {
  if(kondisi_LED == true) {
    if (digitalRead(bt_status) == LOW) {
      digitalWrite(relay_motor, LOW);
    }
    else {
      digitalWrite(relay_motor, LOW);
    }
  }
  else {
    digitalWrite(relay_motor, HIGH);
  }
}

void led_status() {
  if (kondisi_LED == false) {
    digitalWrite(tolak_akses, HIGH);
    Serial.println("Motor ON");
    kondisi_LED = true;
    }
    else {
      digitalWrite(tolak_akses, LOW);
      Serial.println("Motor OFF");
      kondisi_LED = false;
    }
}

void led_verifikasi() {
 digitalWrite(tolak_akses, HIGH); delay(800);
 digitalWrite(tolak_akses, LOW);  delay(800);
}

void led_tolak() {

}

void notifikasi_RFID() {

}