// Francis Masha - Programmable Arduino thermostat.
// by Almond.

#include "DHT.h"
#include <Wire.h>
//#include "rgb_lcd.h"
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <avr/wdt.h> ////watchdog
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16, 4);
//rgb_lcd lcd;
RTC_DS3231 rtc;
DateTime now;

boolean displayAlarmSet = false;
boolean alarm = false;
boolean armed = false;

int alarmHrs = 17;
int alarmMins = 0;
int program = 1;
int list = 0;
unsigned long gap = 10; // gap time 10 min

int alarm1 [11];
int alarm2 [11];
int alarm3 [11];
int alarm4 [11];
int alarm5 [11];
int alarm6 [11];
int alarm7 [11];
int alarm8 [11];
int alarm9 [11];
int alarmHours[9];
int alarmMinutes[9];
int modality;
int OffOnMode [9];
int stormod = 0;
int gapprogram [9];

#define DHTPIN 11     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int tempThreshold = 20; //Set this temperature threshold based upon your preferencies
const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int DS1307 = 0x68; // Address of DS1307 see data sheets

unsigned long count;

boolean DEBUG=true;  // Set to true if you want to read output on the Serial port

int dd = 0;
int ddWeek [] = {0,1,2,3,4,5,6};
int ddOnOff [] = {0,0,0,0,0,0,0};
int OnOffMode;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

const int alarmSetPin=6;
const int incrementAlarmHrsPin=7;
const int incrementAlarmMinsPin=8;
const int setDataPin=5;
const int alarmArmedPin=10;
int relayPin = 11;   // The output pin to actuate the relay

void setup() {

  if(DEBUG){Serial.begin(9600);}
  Wire.begin();
  lcd.init();      // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.print("     ALMONDv.1    ");
  lcd.setCursor(0,1);
  lcd.print(" loading data");
  dht.begin();
  rtc.begin();
  pinMode(11, OUTPUT);
  pinMode(alarmSetPin, INPUT);
  pinMode(incrementAlarmHrsPin, INPUT);
  pinMode(incrementAlarmMinsPin, INPUT);
  pinMode(setDataPin, INPUT);
  pinMode(alarmArmedPin, INPUT);
  digitalWrite(setDataPin,HIGH);
  for (int a=0;a<11;a++){
      alarm1[a] = EEPROM.read(a);
      Serial.print(alarm1[a]);
      Serial.print(".");
      lcd.setCursor(13,1);
      lcd.print(".  ");
      delay(250);

      alarm2[a] = EEPROM.read(a+100);
      Serial.print(alarm2[a]);
      Serial.print(".");
      delay(250);

      alarm3[a] = EEPROM.read(a+200);
      Serial.print(alarm3[a]);
      Serial.print(".");
      delay(250);

      alarm4[a] = EEPROM.read(a+300);
      Serial.print(alarm4[a]);
      Serial.print(".");
      lcd.setCursor(13,1);
      lcd.print(".. ");
      delay(250);

      alarm5[a] = EEPROM.read(a+400);
      Serial.print(alarm5[a]);
      Serial.print(".");
      delay(250);

      alarm6[a] = EEPROM.read(a+500);
      Serial.print(alarm6[a]);
      Serial.print(".");
      delay(250);

      alarm7[a] = EEPROM.read(a+600);
      Serial.print(alarm7[a]);
      Serial.print(".");
      lcd.setCursor(13,1);
      lcd.print("...");
      delay(250);

      alarm8[a] = EEPROM.read(a+700);
      Serial.print(alarm8[a]);
      Serial.print(".");
      delay(250);

      alarm9[a] = EEPROM.read(a+800);
      Serial.print(alarm9[a]);
      Serial.println();
      delay(250);
      const int gapprogram [] = {alarm1 [9], alarm2 [9],
      alarm3 [9], alarm4 [9], alarm5 [9],
      alarm6 [9], alarm7 [9], alarm8 [9], alarm9 [9]};
  }
  Serial.print("READING END");
}

void loop(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  now = rtc.now();
  int alarmArmed=digitalRead(alarmArmedPin);
  if (alarmArmed==HIGH){
    armed=true;
    } else {
    armed=false;
    }
  if(armed){
    if (!alarm){
       checkAlarm();
    }
    else {
       soundAlarm();
    }
  }
  int setMode = digitalRead(alarmSetPin);
  if (setMode==HIGH){
      displayAlarmSet=true;
      setAlarm();
  }
  else {
    displayAlarmSet=false;
    automaticMode();
  }

  if (digitalRead(relayPin)==LOW){
    lcdtime ();
  }
  if(DEBUG){
    Serial.print("Temp: ");
    Serial.println(t);
    Serial.println();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.print(now.dayOfTheWeek(), DEC);
    Serial.println();
  }
}

void automaticMode(){
  float t = dht.readTemperature();
  if (t>tempThreshold){
    digitalWrite(relayPin, LOW);
  }
  if (t<tempThreshold){// && digitalRead(relayPin)==LOW){
    digitalWrite(relayPin, HIGH);
    count=millis();
    }
    unsigned long adesso=millis();
    if (digitalRead(relayPin)==HIGH && t<=tempThreshold && adesso<=count+120000){// 2 min SET GAP TIME FOR AUTOMODE
        lcd.clear();
        lcd.print("   AUTO MODE    ");
        lcd.setCursor(0,1);
        lcd.print("TLNC:"); // show seconds left to the next temperature check
        int difference = (count+120000 - adesso) / 1000 ;
        lcd.print(difference);
        lcd.print("s");
        lcd.print(" T: ");
        lcd.print(t);
        for(int i = 0; i <200; i++){
          //lcd.setRGB(i, 0, 0);
          delay(25);
        }
    }
    else {
      digitalWrite(relayPin, LOW);  // Close the relay
    }
}

void lcdtime () {
  if(displayAlarmSet){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Set program    ");
    lcd.setCursor(0, 1);
    lcd.print("   press Enter  ");
    }
  else {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int o = now.hour();
  int m = now.minute();
  int s = now.second();
  lcd.setCursor(0, 0);
  lcd.print("Ora ");
  if (o < 10){
    lcd.print("0");
  }
  lcd.print(o);
  lcd.print(":");
  if (m < 10){
    lcd.print("0");
  }
  lcd.print(m);
  lcd.print(":");
  if (s < 10){
    lcd.print("0");
  }
  lcd.print(s);
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(t);
  lcd.print(" H: ");
  lcd.print(h);
  lcd.print("% ");
  if (armed){
      lcd.print("ON ");
    }
  else lcd.print("OFF ");
  }
}

void menuProgram() {
  int alarmHours[]={alarm1 [7], alarm2 [7], alarm3 [7],
        alarm4 [7], alarm5 [7], alarm6 [7], alarm7 [7],
        alarm8 [7], alarm9 [7]};
  int alarmMinutes[]={alarm1 [8], alarm2 [8], alarm3 [8],
        alarm4 [8], alarm5 [8], alarm6 [8], alarm7 [8],
        alarm8 [8], alarm9 [8]};
  int OffOnMode [] = {alarm1 [10], alarm2 [10], alarm3 [10],
        alarm4 [10], alarm5 [10], alarm6 [10], alarm7 [10],
        alarm8 [10], alarm9 [10]};
  int gapprogram [] = {alarm1 [9], alarm2 [9],alarm3 [9],
        alarm4 [9], alarm5 [9], alarm6 [9], alarm7 [9],
        alarm8 [9], alarm9 [9]};
  lcd.setCursor(0, 0);
  lcd.print("Set program n.");
  lcd.print(program);
  lcd.setCursor(0, 1);
  lcd.print(alarmHours[program-1]);
  lcd.print(":");
  lcd.print(alarmMinutes[program-1]);
  lcd.print("|");
  lcd.print(OffOnMode [program-1]);
  lcd.print("|");
  lcd.print(gapprogram [program-1]);
  lcd.print(" Min      ");
}

void menuTime(){
  lcd.setCursor(0, 0);
  lcd.print(" Set start time:");
  lcd.setCursor(0, 1);
  lcd.print("    ");
  if (alarmHrs<10){
    lcd.print(0);
  }
  lcd.print(alarmHrs);
  lcd.print(":");
  if (alarmMins<10){
    lcd.print(0);
  }
  lcd.print(alarmMins);
  lcd.print("      ");
}

void menuGap(){
  lcd.setCursor(0, 0);
  lcd.print("  Set duration  ");
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.print(gap);
  lcd.print(" Min     ");
}


void menuOfftime(){
  lcd.setCursor(0, 1);
  lcd.print(" ");
  if (OnOffMode==true){
    lcd.print("ON           ");
    delay(500);
  }
  if (OnOffMode==false){
    lcd.print("           OFF  ");
    delay(500);
  }
}

void menuDow(){
  lcd.setCursor(10,1);
  lcd.print("  ");
  if (ddOnOff[dd]==1){
    lcd.print("ON ");
    delay(500);
  }
  if (ddOnOff[dd]==0){
    lcd.print("OFF ");
    delay(500);
  }

}

void recap(){
  for (int i=0; i<7;i++){
    lcd.setCursor(0,0);
    lcd.print(program);
    lcd.print(alarmHrs);
    lcd.print(alarmMins);
    lcd.print(gap);
    lcd.setCursor(0,1);
    lcd.print(days[i]);
    lcd.print(ddWeek [i]);
    lcd.print(ddOnOff[i]);
    delay(2000);
    }
    buttonPushCounter = 0;
}

void scrollProgram(){
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("SMTWTFS|");
    lcd.print(alarmHours[list]);
    lcd.print(":");
    lcd.print(alarmMinutes[list]);
    lcd.print("|");
    lcd.print(OffOnMode [list]);
    lcd.setCursor(0,1);
    lcd.print("| ");
    lcd.print(gapprogram [list]);
    lcd.setCursor(13,1);
    list+=1;
}


void checkAlarm(){
  int alarmHours[]={alarm1 [7], alarm2 [7], alarm3 [7],
        alarm4 [7], alarm5 [7], alarm6 [7], alarm7 [7],
        alarm8 [7], alarm9 [7]};
  int alarmMinutes[]={alarm1 [8], alarm2 [8], alarm3 [8],
        alarm4 [8], alarm5 [8], alarm6 [8], alarm7 [8],
        alarm8 [8], alarm9 [8]};
  int OffOnMode [] = {alarm1 [10], alarm2 [10], alarm3 [10],
        alarm4 [10], alarm5 [10], alarm6 [10], alarm7 [10],
        alarm8 [10], alarm9 [10]};
  int giset = now.dayOfTheWeek();
  int ora = now.hour();
  int minuti = now.minute();
  int dayWkOn[] = {alarm1 [giset], alarm2 [giset],
      alarm3 [giset], alarm4 [giset], alarm5 [giset],
      alarm6 [giset], alarm7 [giset], alarm8 [giset], alarm9 [giset]};
  int gapprogram [] = {alarm1 [9], alarm2 [9],
      alarm3 [9], alarm4 [9], alarm5 [9],
      alarm6 [9], alarm7 [9], alarm8 [9], alarm9 [9]};
  for (int m=0;m<9;m++){
    if (alarmHours[m]==ora &&  alarmMinutes[m]==minuti && dayWkOn[m]==1){
      alarm=true;
      modality = m;
      m = 9;
    }
  }
  stormod = OffOnMode [modality];
  gap = gapprogram [modality];
}


void setAlarm(){
  int hrs=digitalRead(incrementAlarmHrsPin);
  int mins=digitalRead(incrementAlarmMinsPin);
  buttonState = digitalRead(setDataPin);
  //lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Set program    ");
  lcd.setCursor(0, 1);
  lcd.print("   press Enter  ");
  if (buttonState != lastButtonState){
      delay(500);
      buttonPushCounter++;

      while (buttonPushCounter == 1 ){
        menuProgram();
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          program+=1;
          delay(200);
          if(program>9){
            program=1;
          }
        }
        if (digitalRead(incrementAlarmMinsPin)==HIGH){
            //   funzione cancella programma
            lcd.clear();
            lcd.print(" DELETE ");
            delay(500);
            deleteProgram();
        }
        EnterButton();
        delay(200);
      }
      delay(500);

      while (buttonPushCounter == 2 ){
        menuTime();
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          alarmHrs+=1;
          delay(100);
          if(alarmHrs>23){
            alarmHrs=0;
          }
        }
        if (digitalRead(incrementAlarmMinsPin)==HIGH){
          alarmMins+=5;
          delay(50);
          if(alarmMins>59){
            alarmMins=0;
          }
        }
        EnterButton();
        delay(200);
      }
      delay(500);
      gap=10;

      while (buttonPushCounter == 3 ){
        menuGap();
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          gap+=10;     // Increment the seconds upward
          delay(20);      // Wait a moment between incrementing the numbers
          if(gap>100){ //
            gap=10;
          }
        }
        checkError();
        EnterButton();
        delay(20);
      }
      lcd.clear();
      delay(500);

      while (buttonPushCounter == 4 ){
        delay(500);
        lcd.setCursor(0, 0);
        lcd.print(" ON   MODE  OFF ");
        lcd.setCursor(0, 1);
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          OnOffMode = 1;
          menuOfftime();
          buttonPushCounter = 5;
        }
        if (digitalRead(incrementAlarmMinsPin)==HIGH){
          OnOffMode = 0;
          menuOfftime();
          buttonPushCounter = 5;
        }
        delay(50);
      }
      delay(500);
      dd = 0;

      while (buttonPushCounter == 5 ){
        delay(500);
        lcd.setCursor(0, 0);
        lcd.print("DAYOFWEEK       ");
        lcd.setCursor(0, 1);
        lcd.print(days[dd]);
        lcd.print("  ");
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          ddOnOff [dd] = 1;
          menuDow();
          dd+=1;
          if (dd==7){
            dd=0;
          }
        }
        if (digitalRead(incrementAlarmMinsPin)==HIGH){
          ddOnOff [dd] = 0;
          menuDow();
          dd+=1;
          if (dd==7){
            dd=0;
          }
        }
        EnterButton();
        delay(50);
      }
      delay(500);
      lcd.clear();

      while (buttonPushCounter == 6 ){
        delay(500);
        lcd.setCursor(0, 1);
        if (digitalRead(incrementAlarmHrsPin)==HIGH){
          lcd.setCursor(13,1);
          lcd.print("YES");
          buttonPushCounter = 7;
          delay(500);
        }
        lcd.setCursor(0, 0);
        lcd.setCursor(13,1);
        lcd.print("YES");
        if (digitalRead(incrementAlarmMinsPin)==HIGH){
          lcd.setCursor(13,1);
          lcd.print("NO ");
          buttonPushCounter = 0;
          delay(500);
        }
        delay(500);
        lcd.setCursor(0,0);
        lcd.print("SMTWTFS|");
        lcd.print(alarmHrs);
        if (alarmHrs<10){
          lcd.print("0");
        }
        lcd.print(":");
        lcd.print(alarmMins);
        if (alarmMins<10){
          lcd.print("0");
        }
        lcd.print("|");
        lcd.print(OnOffMode);
        lcd.setCursor(0,1);
        for (int p=0;p<7;p++){
          lcd.print(ddOnOff[p]);
        }
        lcd.print("|");
        lcd.print(gap);
        lcd.print("m");
        lcd.setCursor(13,1);
        lcd.print("NO ");
      }
      lcd.clear();

      while (buttonPushCounter == 7 ){
      delay(200);
      lcd.print(" SAVING . . .");
      program = program - 1;
      program = program*100;
      for (int a=0;a<7;a++){
        EEPROM.write(program, ddOnOff[a]);
        program+=1;
        delay(500);
      }
      Serial.print(program);
      EEPROM.write(program, alarmHrs);
      program+=1;
      delay(500);
      EEPROM.write(program, alarmMins);
      program+=1;
      delay(500);
      EEPROM.write(program, gap);
      program+=1;
      delay(500);
      EEPROM.write(program, OnOffMode);
      delay(500);
      lcd.clear();
      wdt_enable(WDTO_4S);
      delay(1000);
      wdt_reset();
      }
   }
}

void EnterButton(){
  if (digitalRead(setDataPin)==HIGH){
          delay(200);
          buttonPushCounter++;
  }
  delay(50);
}

void soundAlarm() {
  unsigned long start = millis();
  gap = gap * 60000;
  unsigned long end = start + gap;

  while (start  <= end ){
      if (stormod==0){
        digitalWrite(relayPin, LOW);
        lcd.clear();
        lcd.print("   STOP MODE    ");
        lcd.setCursor(0,1);
        lcd.print("  mins left: ");
        int difference = (end - start) / 1000 / 60;
        lcd.print(difference);
        Serial.print("                        ");
        Serial.print(difference);
        for(int i = 0; i <200; i++){
          //lcd.setRGB(0, i, 0);
          delay(25);
        }
      }
      if (stormod==1){
        digitalWrite(relayPin, HIGH); // Open the relay
        lcd.clear();
        lcd.print("  HEATING MODE  ");
        lcd.setCursor(0,1);
        lcd.print("  mins left: ");
        int difference = (end - start) / 1000 / 60;
        lcd.print(difference);
        Serial.print("                        ");
        Serial.print(difference);
        for(int i = 0; i <200; i++){
          //lcd.setRGB(i, 0, 0);
          delay(25);
        }
      }
      delay(1000);
      start = millis();
      int hrs=digitalRead(incrementAlarmHrsPin);
      int mins=digitalRead(incrementAlarmMinsPin);
      if (hrs==HIGH || mins==HIGH){
          alarm=false;
      }
      digitalWrite(relayPin, LOW);  // Close the relay
      alarm=false;
  }
}

void checkError(){
  int alarmHours[]={alarm1 [7], alarm2 [7], alarm3 [7],
        alarm4 [7], alarm5 [7], alarm6 [7], alarm7 [7],
        alarm8 [7], alarm9 [7]};
  int alarmMinutes[]={alarm1 [8], alarm2 [8], alarm3 [8],
        alarm4 [8], alarm5 [8], alarm6 [8], alarm7 [8],
        alarm8 [8], alarm9 [8]};
  int gapprogram []={alarm1 [9], alarm2 [9], alarm3 [9],
        alarm4 [9], alarm5 [9], alarm6 [9], alarm7 [9],
        alarm8 [9], alarm9 [9]};
  unsigned long orainminuti = (alarmHrs*3600L)+(alarmMins*60L);
  unsigned long oraminutitempogap = (orainminuti) + (gap*60L);
  unsigned long int memhour = 0;
  unsigned long int memgp = 0;
  for (int d=0;d<9;d++){
        memhour=(alarmHours[d]*3600L)+(alarmMinutes[d]*60L);
        memgp=memhour+gapprogram[d]*60L;
        if (orainminuti >=memhour && orainminuti<=memgp ||
            oraminutitempogap>=memhour && oraminutitempogap<=memgp ||
            orainminuti<=memhour && oraminutitempogap>=memgp){
            lcd.clear();
            lcd.print("ERROR PROG.N.");
            lcd.print(d+1);
            lcd.setCursor(0,1);
            lcd.print(" ");
            lcd.print(alarmHours[d]);
            lcd.print(":");
            lcd.print(alarmMinutes[d]);
            delay(1500);
            buttonPushCounter = 0;
        }
  }
}

void deleteProgram(){
  int memoPos = (program - 1) * 100;
  lcd.clear();
  lcd.print("    DELETING    ");
  lcd.setCursor(0,1);
  lcd.print("Prog.");
  lcd.print(program);
  lcd.print(" Mem.n.");
  lcd.print(memoPos);
  delay(1000);
  for (int v=0;v<11;v++){
    EEPROM.write(memoPos+v, 0);
    delay(500);
  }
  wdt_enable(WDTO_4S);
  wdt_reset();
}
