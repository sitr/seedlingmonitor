#include <SimpleTimer.h>
#include <DHT.h>

#define DHTTYPE DHT22

const short tempHumSensorPin = 4;

DHT dht(tempHumSensorPin, DHTTYPE);

void setup(void) {
   delay(1000);
   dht.begin();

   Serial.begin(115200);
   timer.setInterval(10000, readSensorValues);
}

void readSensorValues() {
   float t = dht.readTemperature();
   float h = dht.readHumidity();
   Serial.print("Temperature = ");
   Serial.print(t);
   Serial.print(" *C ");
   Serial.print("    Humidity = ");
   Serial.print(h);
   Serial.println(" % ");
}

void loop(void) {
   timer.run();
}