#include <DHT_U.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
 
DHT_unified dht(DHTPIN,DHTTYPE);

void setup(){
  
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.tempreture().getSernsor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  
}
  
void loop(){
 
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {                                                // temprerature     
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
 
  dht.humidity().getEvent(&event);                      // Humidity 
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  
}
