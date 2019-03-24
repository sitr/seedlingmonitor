#include <SimpleTimer.h>
#include <DHT.h>
#include <PCD8544.h>

#define DHTTYPE DHT22

const short tempHumSensorPin = 8;

DHT dht(tempHumSensorPin, DHTTYPE);
SimpleTimer timer;
PCD8544 lcd;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

void setup() {
   dht.begin();
   lcd.begin(84, 48);
   lcd.setContrast(60);
   lcd.createChar(0, degrees_glyph);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Seedling controller");
   delay(5000);
   lcd.clear();
   timer.setInterval(10000, readSensorValues);
}

void readSensorValues() {
   float temp = dht.readTemperature();
   float hum = dht.readHumidity();

   lcd.setCursor(0,0);
   lcd.print("Temp: ");
   lcd.print(temp, 1);
   lcd.print(" ");
   lcd.write(0);
   lcd.print("C ");
   lcd.setCursor(0,1);
   lcd.print("Fukt: ");
   lcd.print(hum, 1);
   lcd.print(" % ");
}

void loop(void) {
   timer.run();
}