#include <SimpleTimer.h>
#include <DHT.h>
#include <LCD5110_Graph.h>
#include <x10rf.h>

#define DHTTYPE DHT22

extern uint8_t splashScreen[];
const short lcdPinCLK = 8;
const short lcdPinDIN = 9;
const short lcdPinDC = 10;
const short lcdPinRST = 11;
const short lcdPinCE = 12;

const short tempHumSensorPin = 1;
const short phSensorPin = A0;
const short txPin = 2;
const short reps = 1;

extern uint8_t SmallFont[];
extern uint8_t TinyFont[];

DHT dht(tempHumSensorPin, DHTTYPE);
SimpleTimer timer;
LCD5110 lcd(lcdPinCLK, lcdPinDIN, lcdPinDC, lcdPinRST, lcdPinCE);
x10rf myx10 = x10rf(txPin,13,reps);

static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

void setup() {
   dht.begin();
   myx10.begin();
   lcd.InitLCD();
   lcd.drawBitmap(0, 0, splashScreen, 84, 48);
   lcd.update();
   delay(5000);
   lcd.clrScr();
   lcd.setFont(SmallFont);

   readSensorValues();
   timer.setInterval(3000, readSensorValues);
}

void readSensorValues() {
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();
   float ph = readPhValue();
   delay(20);

   String tempString = "Tmp: " + String(temperature, 1) + " C";
   String humString = "Hum: " + String(humidity, 1) + "%";
   String phString = "pH:  " + String(ph, 1);

   lcd.print(tempString, 0, 0);
   lcd.print(humString, 0, 12);
   lcd.print(phString, 0, 24);
   lcd.update();
  // sendData(temperature, humidity);
}

float readPhValue() {
   unsigned long int avgPhValue;
   int buf[10], temp;
   for(int i = 0; i < 10; i++)
   {
      buf[i]=analogRead(phSensorPin);
      delay(10);
   }
   for(int i = 0; i < 9; i++)
      {
      for(int j = i + 1; j < 10; j++)
      {
         if(buf[i] > buf[j])
         {
            temp=buf[i];
            buf[i]=buf[j];
            buf[j]=temp;
         }
      }
   }
   avgPhValue = 0;
   for(int i = 2; i < 8; i++)
   avgPhValue += buf[i];
   float pHVol= (float)avgPhValue * 5.0 / 1024 / 6;
   float phValue = -5.70 * pHVol + 21.34;

   return phValue;
}

void sendData(float temperature, float humidity){
   myx10.RFXsensor(1, 't', 't', temperature);
   delay(250);
   myx10.RFXsensor(2, 't', 'h', humidity);
   delay(250);
}

void loop() {
   timer.run();
}