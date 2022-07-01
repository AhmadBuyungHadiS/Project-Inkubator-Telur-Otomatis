#include <Wire.h> // Library Wire.h
#include "RTClib.h" //Library RTC 1307
#include <LiquidCrystal_I2C.h> //Library LCD I2C
#include <DHT.h> //Library DHT 11
LiquidCrystal_I2C lcd(0x27 ,16,2); //Inisialisasi alamat modul I2C
#define DHTPIN 2 //Inisialisasi pin DHT 11 di Pin No 2 Arduino
#define DHTTYPE DHT11 //Inisialisasi tipe DHT 
DHT dht(DHTPIN,DHTTYPE);

  //Inisialisasi Pin Sensor Ultrasonik HC SR04
  int trig = 12;           // membuat variabel trig yang di set ke-pin 12
  int echo = 13;           // membuat variabel echo yang di set ke-pin 13
  long duration;
  long distance;

  //Inisialisasi pin Relay
  const int ptc = 3; // Pin relay 1 di pin 3 arduino
  const int fan = 4; // Pin relay 2 di pin 4 arduino
  const int exhaust = 5; // Pin relay 3 di pin 5 arduino
  const int pump = 6; // Pin relay 4 di pin 6 arduino
  const int motor = 7; // Pin relay 5 di pin 7 arduino

  //Inisialisasi pin buzzer
  const int buzzer = 8;

  //Setup RTC 1307
  RTC_DS1307 RTC;
  char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Satur"};

void setup(){
    Serial.begin(9600);
    //Memulai sensor dht 11
    dht.begin();

    //Setting LCD 16x2 I2C
    lcd.init();
    lcd.backlight();

    //Setting pin Mode sensor ultrasonik HC SR04
    pinMode(trig, OUTPUT);    // set pin trig menjadi OUTPUT
    pinMode(echo, INPUT);     // set pin echo menjadi INPUT

    //Setting pin Mode Relay
    pinMode(ptc, OUTPUT);
    pinMode(fan, OUTPUT);
    pinMode(exhaust, OUTPUT);
    pinMode(pump, OUTPUT);
    pinMode(motor, OUTPUT);
    
    digitalWrite(ptc, HIGH);
    digitalWrite(fan, HIGH);
    digitalWrite(exhaust, HIGH);
    digitalWrite(pump, HIGH);
    digitalWrite(motor, HIGH);

    //Inisialisasi pin Mode buzzer
    pinMode(buzzer, OUTPUT);

    lcd.setCursor(1, 0);   
    lcd.print("Projek TA 2022");
    lcd.setCursor(4,1);
    lcd.print("MIF 2019");
    delay(1000);
    lcd.clear();
   
    lcd.setCursor(0,0);
    lcd.print("Ahmad Buyung Hadi Santoso");
    lcd.setCursor(0,1);
    lcd.print("NIM : E31191793");
    delay(1000);
    //Membuat efek teks running ke kanan
    for ( int i = 1; i <= 40; i++ ){
      lcd.scrollDisplayRight();
      delay(500);
    }
    delay(1000);
    lcd.clear(); 

    RTC.begin();
    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        lcd.setCursor(0,0);
        lcd.print("RTC is NOT running!");
        lcd.clear();
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}    

void loop(){  
    rtc();
    delay(2000);
    lcd.clear();
    temperature(); 
    volume();
    delay(2000);
    lcd.clear();
    putar();  
     
}

void rtc(){
    /* PENGATURAN TANGGAL, BULAN, TAHUN */
    DateTime now = RTC.now();
    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.print(',');
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.print(' ');

    /* PENGATURAN JAM, MENIT, DETIK */
    lcd.setCursor(0, 1);
    if (now.hour() >= 0 && now.hour() < 10) {
    lcd.print('0');
    }
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute() >= 0 && now.minute() < 10) {
    lcd.print('0');
    }
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second() >= 0 && now.second() < 10) {
    lcd.print('0');
    }
    lcd.print(now.second(), DEC);

    /*
    Serial.print("JAM : ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);*/
}

void putar(){
  DateTime now = RTC.now();
  if (now.hour() == 6 && now.minute() ==00 && now.second() < 10){
    digitalWrite(motor, LOW);
    delay(6000);
    digitalWrite(motor, HIGH);
    delay(1000);
  }
   if (now.hour() == 12 && now.minute() ==00 && now.second() < 10){
    digitalWrite(motor, LOW);
    delay(6000);
    digitalWrite(motor, HIGH);
    delay(1000);
  }
   if (now.hour() == 18 && now.minute() ==00 && now.second() < 10){
    digitalWrite(motor, LOW);
    delay(6000);
    digitalWrite(motor, HIGH);
    delay(1000);
  }
   if (now.hour() ==00 && now.minute() ==00 && now.second() < 10){
    digitalWrite(motor, LOW);
    delay(6000);
    digitalWrite(motor, HIGH);
    delay(1000);
  }  
}

void temperature(){
   //Pembacaan Suhu dan Kelembaban
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    //mengecek pembacaan apakah terjadi kegagalan atau tidak
    if (isnan(hum) || isnan(temp)) {
      Serial.println("Pembacaan data dari module sensor gagal!");
      return;
    }

    /*
    Serial.print("Hum: ");
    Serial.print(hum);
    Serial.print(" %\t");
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C\t");
    delay(1000);*/
    
    lcd.setCursor(0,0);
    lcd.print("H:");
    lcd.print(hum);
    lcd.setCursor(8,0);
    lcd.print("T:");
    lcd.print(temp);
    delay(1000);

    if ( temp <= 37 ) {
      digitalWrite(ptc, LOW);
      digitalWrite(fan, LOW);
      digitalWrite(exhaust, HIGH);
    }

    if ( temp >= 39 ) {
      digitalWrite(ptc, HIGH);
      digitalWrite(fan, HIGH);
      digitalWrite(exhaust, LOW);
    }

    if ( temp <= 35 ) {
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
    }

    if ( temp >= 41 ) {
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
    }

    if ( hum <= 45 ) {
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
    }

    if ( hum >= 70 ) {
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
    }
}

void volume(){
    // program dibawah ini agar trigger memancarakan ultrasonic
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
  
    duration = pulseIn(echo, HIGH); // menerima suara ultrasonic
    distance = (duration / 2) / 29.1; // mengubah durasi menjadi jarak (cm)
    Serial.println(distance);  // menampilkan jarak pada Serial Monitor

    lcd.setCursor(0,1);
    lcd.print("Jarak:");
    lcd.print(distance);
    lcd.print("cm");
    delay(1000);

    if ( distance == 8 ) {
      digitalWrite(pump, LOW); 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pompa");
      lcd.setCursor(0,1);
      lcd.print("Hidup");
      delay(60000);
      lcd.clear();
    }
    
    if ( distance <= 7 ) {
      digitalWrite(pump, HIGH);
    }
}
