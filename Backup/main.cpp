//By Francis Masha

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "ACS712.h"
#include <SHT1X.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SleepyDog.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"
//#include "DHT.h"

#define RED_LED 5
#define BLUE_LED 6
#define FONA_RX  2
#define FONA_TX  3
#define FONA_RST 4
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
//#define DHTPIN 3
//#define DHTTYPE DHT11

const int BUZZER = 7;
const int FAN = 8;
const int PUMP = 11;

//Variables for storing values
int32_t lux = 0;
float temp = 0;
float humid = 0;
float voltage = 12.0;
float amps = 0.0;
float totAmps = 0.0;
float avgAmps = 0.0;
float ampHour = 0.0;
float watt = 0.0;
float power = 0.0;
float energy = 0.0;

Time t;

//Sensors and device I2C address
BH1750 lightMeter(0x23);
LiquidCrystal_I2C lcd(0x27,16, 4);

SHT1x sht15(3, 4);//Data, SCK
ACS712 sensor(ACS712_30A, A0);
DS3231  rtc(SDA, SCL);
//DHT dht(DHTPIN, DHTTYPE);

/************************* WiFi Access Point *********************************/
#define FONA_APN       ""
#define FONA_USERNAME  ""
#define FONA_PASSWORD  ""

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "mashafrancis"
#define AIO_KEY         "ac0b72ffff774cd099658d976dae46e9"

/************ Global State (you don't need to change this!) ******************/

// Setup the FONA MQTT class by passing in the FONA class and MQTT server and login details.
Adafruit_MQTT_FONA mqtt(&fona, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

// FONAconnect is a helper function that sets up the FONA and connects to
// the GPRS network. See the fonahelper.cpp tab above for the source!
boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humid");
Adafruit_MQTT_Publish Light = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lux");
Adafruit_MQTT_Publish Energy = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/energy");

/*************************** Sketch Code ************************************/

// How many transmission failures in a row we're willing to be ok with before reset
uint8_t txfailures = 0;
#define MAXTXFAILURES 3

void setup() {
  Serial.begin(115200);
  sensor.calibrate();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  rtc.begin();

  pinMode(RED_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);

  lcd.init();      // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight

  //dht.begin();
  while (!Serial);

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();

  // Initialise the FONA module
  while (! FONAconnect(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD))) {
    Serial.println("Retrying FONA");
  }

  Serial.println(F("Connected to Cellular!"));

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();
}

void dcPower(){
  long millisec = millis();     //Calculate time in milliseconds
  long time = millisec/1000;    //Convert milliseconds to seconds
  amps = sensor.getCurrentDC();
  totAmps = totAmps+amps;       //Calculate total avgAmps
  avgAmps = totAmps/time;       //Average amps
  ampHour = (avgAmps*time)/3600; //ampere-AmpHour
  watt = voltage*amps;          //Power = V * I
  energy = (watt*time)/3600;
}

void climate(){
  temp = sht15.readTemperatureC();
  humid = sht15.readHumidity();
  lux = lightMeter.readLightLevel();

  /*temp = dht.readTemperature();
  humid = dht.readHumidity();
  if (isnan(humid) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;*/
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
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

void pumpTime(){
  if(t.hour == 8 && t.min == 00){
    digitalWrite(PUMP,HIGH);
    Serial.println("PUMP ON");
    }

    else if(t.hour == 8 && t.min == 15){
      digitalWrite(PUMP,LOW);
      Serial.println("PUMP OFF");
    }
}

void lcdDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(humid);
  lcd.print(" %");
}

void dataSend(){
  // Make sure to reset watchdog every loop iteration!
  Watchdog.reset();

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  Watchdog.reset();
  // Now we can publish stuff!
  // Serial.print(x);
  Serial.print("...");
  if (! Temperature.publish(temp)) {
    Serial.println(F("Failed"));
    txfailures++;
  } else {
    Serial.println(F("OK!"));
    txfailures = 0;
  }

  if (! Humidity.publish(humid)) {
    Serial.println(F("Failed"));
    txfailures++;
  } else {
    Serial.println(F("OK!"));
    txfailures = 0;

  if (! Light.publish(lux)) {
    Serial.println(F("Failed"));
    txfailures++;
  } else {
    Serial.println(F("OK!"));
    txfailures = 0;
  }

  if (! Energy.publish(energy)) {
    Serial.println(F("Failed"));
    txfailures++;
  } else {
    Serial.println(F("OK!"));
    txfailures = 0;
  }

  Watchdog.reset();
  // this is our 'wait for incoming subscription packets' busy subloop
  // ping the server to keep the mqtt connection alive, only needed if we're not publishing
  //if(! mqtt.ping()) {
  //  Serial.println(F("MQTT Ping failed."));
  //}
  }
}

uint32_t x=0;

void loop() {
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");

  pumpTime();

  dcPower();
  climate();
  lcdDisplay();

  if(temp >= 26){
      blue_off();
      red_on();
      red_off();
      tone(BUZZER, 1000);
      delay(1500);
      digitalWrite(FAN, HIGH);
      //one_time = true;
      //Turn the fan on and update display
  }
  else{
      red_off();
      blue_on();
      noTone(BUZZER);
      digitalWrite(FAN, LOW);
  }

  Serial.print(" Temp = ");
  Serial.print(temp);
  Serial.println("C");
  Serial.print(" Humidity = ");
  Serial.print(humid);
  Serial.println("%");

  dataSend();
}
