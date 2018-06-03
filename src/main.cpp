//By Francis Masha Konde

#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SleepyDog.h>

#define RED_LED 8
#define BLUE_LED 7

const int BUZZER = 4;
const int PUMP = 12;

Time t;

//Sensors and device I2C address
LiquidCrystal_I2C lcd(0x27,16, 4);
DS3231  rtc(SDA, SCL);

const int pumpHr0 = 0;
const int pumpHr1 = 1;
const int pumpHr2 = 2;
const int pumpHr3 = 3;
const int pumpHr4 = 4;
const int pumpHr5 = 5;
const int pumpHr6 = 6;
const int pumpHr7 = 7;
const int pumpHr8 = 8;
const int pumpHr9 = 9;
const int pumpHr10 = 10;
const int pumpHr11 = 11;
const int pumpHr12 = 12;
const int pumpHr13 = 13;
const int pumpHr14 = 14;
const int pumpHr15 = 15;
const int pumpHr16 = 16;
const int pumpHr17 = 17;
const int pumpHr18 = 18;
const int pumpHr19 = 19;
const int pumpHr20 = 20;
const int pumpHr21 = 21;
const int pumpHr22 = 22;
const int pumpHr23 = 23;
const int OnMin = 00;
const int OffMin = 15;

void setup() {
  Serial.begin(115200);
  rtc.begin();

  pinMode(RED_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);

  lcd.init();      // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight

}

void buzzerAlarm(){
  tone(BUZZER, 1000);
}

void red_on(){
  digitalWrite(RED_LED,HIGH);
    delay(1000);}

void red_off(){
   digitalWrite(RED_LED,LOW);
    delay(1000);}

void blue_on(){
    digitalWrite(BLUE_LED,HIGH);
    delay(2000);}

void blue_off(){
   digitalWrite(BLUE_LED,LOW);
    delay(1000);}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.

void pumpTime(){
  if(t.hour == pumpHr0 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr0 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr1 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr1 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr2 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr2 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr3 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr3 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr4 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr4 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr5 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr5 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr6 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr6 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr7 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr7 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr8 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr8 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr9 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr9 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr10 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr10 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr11 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr11 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr12 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr12 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr13 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr13 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr14 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr14 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr15 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr15 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr16 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr16 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr17 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr17 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr18 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr18 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr19 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr19 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr20 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr20 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr21 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr21 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr22 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr22 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

  if(t.hour == pumpHr23 && t.min == OnMin){
    digitalWrite(PUMP,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    Serial.println("PUMP ON");}

    else if(t.hour == pumpHr23 && t.min == OffMin){
      digitalWrite(PUMP,LOW);
      digitalWrite(BLUE_LED,LOW);
      Serial.println("PUMP OFF");}

}

void lcdDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  //lcd.print(temp);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  //lcd.print(humid);
  lcd.print(" %");
}

void loop() {
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");

  pumpTime();

}
