/*
 @author - Himanshu kohale/Binarylinux
 Gitub   - https://github.com/Himanshukohale22
 Date - 2024


 *dht11 - Temperature and Humidity
 *MQ2 - CO/LPG/SMOKE
 *MQ8 - H2
 *MQ135  - CO2 , acetone , toluene ,NH4 
 *pm 2.5 - Pm2.5 particle
 *MQ135 - AQI 
 
Display GUI
ESP32 to Arduino data transfer 


 */

/******************************Includes****************************************/
#include <stdio.h>
#include <MQ2.h>
#include <Wire.h>
#include <MQUnifiedsensor.h>
#include "DHT.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

/************pins/variables/functions declairation*****************************/

#define BUZZER 4 // Buzzer for alarm when SMOKE is detected than usually set value (smoke = 100ppm )

#define DHTPIN 3          // DHT11 
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
float h ;
float t ;
float hic ;


int measurePin = A4;      // GYP pm2.5 dust sensor 
int ledPower = 2;         // Digital pin 
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
float H2;

#define Mq_135pin A5            // MQ135 for less accurate AQI  
float AQI;
float CO2;

#define TFT_CS        10        // Display pins
#define TFT_RST        8 
#define TFT_DC         9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ADDING CSV FILE 

// FILE *Datafile;



/****************************main body*****************************************/

void setup() {
   Serial.begin(9600);
   pinMode(7, OUTPUT);

   digitalWrite(7,HIGH);
   delay(500);
   digitalWrite(7,LOW);

   Serial.print("Humidity (%), ");  
   Serial.print("Temperature (°C), "); 
   Serial.print(" Heat index (°C), ");
   Serial.print("LPG (ppm), ");
   Serial.print("CO (ppm), ");
   Serial.print("Smoke (ppm), ");
   Serial.print("H2 (ppm), ");
   Serial.print("CO2 (ppm), ");
   Serial.print("Toluene (ppm), ");
   Serial.print("NH4 (ppm), ");
   Serial.print("Acetone (ppm), ");
   Serial.print("Dust Density (ug/m3), ");
   Serial.print("AQI (ppm), \n");
  
  dht.begin();
  
  pinMode(ledPower, OUTPUT);
  
  mq2.begin();
  
  MQ8.init();
  MQ8.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ8.setR0(0.91);
  
  MQ135.init();
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.setR0(9.03);

  // Serial.print(" Starting \n");  // starting Display 

  tft.initR(INITR_BLACKTAB); 
  tft.initR(INITR_GREENTAB);

  testdrawcircles(10,ST7735_WHITE);
  delay(500);
  testdrawcircles(5,ST7735_WHITE);
  delay(500);
  starting_display();
  delay(3000);
  // tftPrintTest();

}

void loop() {
  
  delay(2000);
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  hic = dht.computeHeatIndex(t);

  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(hic);
  Serial.print(",");

  float* values= mq2.read(true);    
                      
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();

  // alarmSystem(); /  // function will check if value of smoke is incresed, for fire or not 

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
  Serial.print(",");
  Serial.print(Toluene);
  Serial.print(",");
  Serial.print(NH4);
  Serial.print(",");
  Serial.print(Acetone);
  Serial.print(",");
  
  digitalWrite(ledPower, LOW);      
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);   
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 170 * calcVoltage - 0.1;
  
  Serial.print(dustDensity);
  Serial.print(",");  
  
  AQI = analogRead(A3);   // mq135 
  Serial.print(AQI);
  Serial.print("\n");

  tftPrintTest();   // Display
  alarmSystem();    // Alarm system
  delay(5000);

}

/****************************Declaired functions***********************************************/

unsigned long starting_display(){
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);
  // tft.drawLine(0,20, 500,20 ,ST7735_WHITE);
  tft.drawRect(10,10,140,105,ST7735_WHITE);
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(30, 40);
  tft.print("Hey hi!!");
  tft.setCursor(30, 60);
  tft.print("This is");
  tft.setCursor(40, 80);
  tft.print(" VAAYU");  
}

unsigned long tftPrintTest() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);
  tft.drawRect(5,5,150,122,ST7735_WHITE);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(45,10);
  tft.println("VAAYU");
  tft.drawLine(6,27, tft.width()-6, 27, ST7735_WHITE);
  tft.setCursor(8, 35);
  tft.println("Temp :");tft.setCursor(80, 35);tft.print(t);
  tft.setCursor(8, 55);
  tft.println("Humi :");tft.setCursor(80, 55);tft.print(h);
  tft.setCursor(8, 75);
  tft.println("AQI  :");tft.setCursor(80, 75);tft.print(AQI/8);
  tft.setCursor(8, 95);
  tft.println("pm2.5:");tft.setCursor(80, 95);tft.print(dustDensity);
  // tft.setCursor(8, 90);
  // tft.println("H2   :");tft.setCursor(80, 90);tft.print();
  // tft.setCursor(8, 110);
  // tft.println("CO2  :");tft.setCursor(60, 110);tft.print();
  // delay(5000);


  // tft.fillScreen(ST77XX_BLACK);
  // tft.setRotation(3);

  // tft.drawRect(5,5,150,122,ST7735_WHITE);
  // tft.setTextColor(ST7735_WHITE);
  // tft.setTextSize(2);
  // tft.setCursor(8, 10);
  // tft.println("Temp :");tft.setCursor(80, 10);tft.print(co);
  // tft.setCursor(8, 30);
  // tft.println("Humi :");tft.setCursor(80, 30);tft.print();
  // tft.setCursor(8, 50);
  // tft.println("AQI  :");tft.setCursor(80, 50);tft.print(AQI);
  // tft.setCursor(8, 70);
  // tft.println("pm2.5:");tft.setCursor(80, 70);tft.print(dustDensity);

}

// function for buzzer alarm system 

unsigned long alarmSystem(){         

  if (smoke >=5) {digitalWrite(7, HIGH);}
}
// function for sending data to ESP web server Via serial 

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}