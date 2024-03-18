
/*
 *
 *dht11 - Temperature and Humidity
 *MQ2 - CO/LPG/SMOKE
 *MQ8 - H2
 *MQ135  - CO2 , acetone , toluene ,NH4 
 *pm 2.5 - Pm2.5 particle
 *MQ135 - AQI 
 
 */



#include <MQ2.h>
#include <Wire.h>
#include <MQUnifiedsensor.h>
#include "DHT.h"

/***pin declairation****/

#define DHTPIN 3          // DHT11 
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

int measurePin = A4;      // GYP pm2.5 dust sensor 
int ledPower = 2;      
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

#define Board    ("Arduino UNO")    // calibration for MQ8 and MQ135 sensors 
#define Pin8     (A1)  
#define Pin135   (A2)  


#define RatioMQ8CleanAir   (70)  //RS / R0 = 70 ppm 
#define RatioMQ135CleanAir (3.6) //RS / R0  = 10 ppm 

#define ADC_Bit_Resolution (10) // 10 bit ADC 
#define Voltage_Resolution (5) //  Volt resolution to calc the voltage
#define Type               ("Arduino  UNO") 

MQUnifiedsensor MQ8(Board, Voltage_Resolution,  ADC_Bit_Resolution, Pin8, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution,  ADC_Bit_Resolution, Pin135, Type);

int pin = A0;                   // MQ2 sensor for smoke detection 
int lpg, co, smoke;
MQ2 mq2(pin);

#define Mq_135pin A3            // MQ135 for less accurate AQI  
int AQI;


void setup() {
   Serial.begin(9600);

   Serial.print("Humidity, ");  
   Serial.print("Temperature, "); 
   Serial.print(" Heat index, ");
   Serial.print("LPG, ");
   Serial.print("CO, ");
   Serial.print("Smoke, ");
   Serial.print("H2, ");
   Serial.print("CO2, ");
   Serial.print("Toluene, ");
   Serial.print("NH4, ");
   Serial.print("Acetone, ");
   Serial.print("Dust Density, ");
   Serial.print("AQI, \n");
  
  dht.begin();
  
  pinMode(ledPower, OUTPUT);
  
  mq2.begin();
  
  MQ8.init();
  MQ8.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ8.setR0(0.91);
  
  MQ135.init();
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.setR0(9.03);

}

void loop() {
  
  delay(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t);

  Serial.print(h);
  Serial.print("% |");
  Serial.print(t);
  Serial.print("°C |");
  Serial.print(hic);
  Serial.print("°C |");

  float* values= mq2.read(true);    
                      
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();

  MQ8.update();
  MQ135.update();
  MQ8.setA(976.97);  MQ8.setB(-0.688); // H2
  float H2 = MQ8.readSensor();
  Serial.print(H2);

  MQ135.setA(110.47); MQ135.setB(-2.862);  //CO2 
float CO2 = MQ135.readSensor(); 
  
  MQ135.setA(44.947); MQ135.setB(-3.445);  // Toluene
float Toluene = MQ135.readSensor(); 
  
  MQ135.setA(102.2 );  MQ135.setB(-2.473); //NH4 
float NH4 = MQ135.readSensor(); 
  
  MQ135.setA(34.668);  MQ135.setB(-3.369); //Acetone
float Acetone = MQ135.readSensor(); 
  Serial.print(CO2);
  Serial.print("ppm |");
  Serial.print(Toluene);
  Serial.print("ppm |");
  Serial.print(NH4);
  Serial.print("ppm |");
  Serial.print(Acetone);
  Serial.print("ppm |");
  
  digitalWrite(ledPower, LOW);      
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);   
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 170 * calcVoltage - 0.1;
  
  Serial.print(dustDensity);
  Serial.print(" ug/m3 | ");  
  
  AQI = analogRead(A3);   // mq135 
  Serial.print(AQI);
  Serial.print("ppm \n");
  delay(1000);

}
