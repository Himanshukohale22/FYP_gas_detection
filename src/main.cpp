#include <Arduino.h>
#include <Wire.h>

/*
MQ-2 - smoke
MQ-7 - Carbon Monoxide
MQ-8 - Hydrogen Gas
MQ135 - CO
MQ138 - Formaldehyde gas
PM2.5 sensor - partical sensor
CCS811 - Air quality eCO2 400 - 8192 ppm/TVOC 0-1187ppb   I2C
SGP30 - TVOC-0-1000ppm , CO2-eq-0-1000ppm				  I2C
BME680 - tempreture and humidity.						  I2C
*/


int Mq2pin = A0;       // smoke detection
//configuration for co2 and acetone with mq135 sensor
int Mq135pin = A1;
int R0 = 76;
int R2 = 1000;
float RS;
float Acetone_ppm;
int Mq8pin = A2;
int Mq7pin = A3;
int Mq138 = A4;
int delta = 2;
int oldvalue= 13;
int steps = 4;
int RXD =DD1;  // partical sensor uart serial connector
int TXD =DD0;
// air quality index adafruit sensors


void setup() {
	Serial.begin(9600);
	
}
struct pms5003data
{
		uint16_t framelen;
		uint16_t pm10_standard,pm25_standard,pm100_standard;
		uint16_t pm10_env,pm25_env,pm100_env;
		uint16_t particles_03um,particles_05um,particles_10um,particles_25um,particles_50um,particles_100um;
		uint16_t unused;
		uint16_t checksum;

};

struct  pms5003data data;
	
void loop() {

	// for smoke detection 
	int sensorValue_Mq2 = analogRead(Mq2pin);
	Serial.println("smoke detected : ");
	Serial.println(sensorValue_Mq2);

	// for co2 and acetone 
	int sensorValue_Mq135 = analogRead(Mq135pin);
	Serial.println("CO2: ");
	Serial.println(sensorValue_Mq135);

	float volts = volts/1023;
	RS = R2*(1-volts);
	RS = RS/volts;
	Acetone_ppm = 159.6-133.33*(RS/R0);
	Serial.println(Acetone_ppm);

	// corbon monoxide 
	int sensorValue_Mq7 = analogRead(Mq7pin);
	Serial.println("CO: ");
	Serial.println(sensorValue_Mq7);

	// H2 gas 
	int sensorValue_Mq8 = analogRead(Mq8pin);
	Serial.println("H2: ");
	Serial.println(sensorValue_Mq8);

	int sensorValue_Mq138 = analogRead(Mq135pin);
	float voltage = sensorValue_Mq135*(5.0/1023.0);

	steps = (steps+1)%10;

	if (steps==0)
	{
		delta = sensorValue_Mq135-oldvalue;
		if (delta<0)
		{
			delta = -1*delta;
		}
		oldvalue= sensorValue_Mq135;

	}
	Serial.print("ADC: ");
	Serial.print(sensorValue_Mq135);
	Serial.print("   voltage(V); ");
	Serial.println(voltage);

	// PM2.5 partical sensor

	if (readPMSdata(&Serial))
	{
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
  	}
		
}


boolean readPMSdata(Stream*s){
		if (!s->available())
		{
			return false;
		}

		if (s-> peek()!=0x42)
		{
			return false;
		}
		

		if (s->available()<32)
		{
			return false;
		}
		
		uint8_t buffer[32];
		uint16_t sum=0;
		s->readBytes(buffer,32);

		for (uint8_t i = 0; i < 30; i++)
		{
			sum+=buffer[i];
		}


		uint16_t buffer_u16[15];
		for (uint8_t i = 0; i < 15; i++)
		{
			buffer_u16[i] = buffer[2+i * 2+1];
			buffer_u16[i] += (buffer[2+i * 2] << 8);
		}

		memcpy((void *)&data, (void *)buffer_u16,30);  // function used to copy block of memory from one to other memory location
		if (sum!=data.checksum)							// != not equal to
		{
			Serial.println("checksum failure");
			return false;
		}
		
}


