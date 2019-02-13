

//Connection

/*
RTC - ESP8266
SDA - D2
SCL - D1
GND - GND
VCC - 3.3V
*/

/*
DHT22 - ESP8266
OUT/DATA - D3
GND - GND
VCC - 3.3V
*/

/*
SoilMoister -ESP8266
AO - A0
GND - GND
VCC - 3.3V
*/

// Include Libraries
#include <DHTesp.h>
#include "Arduino.h"
#include "RTClib.h"

// Pin Definitions
#define DHT_PIN_DATA D3
#define SOILMOISTURE3V3_PIN_SIG A0 /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

DHTesp dht;
RTC_DS1307 RTC;
double analogVolts = 0.0;
double analogValue = 0.0;

int sensorValue = 0;
int percent = 0;
// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup()
{
  // Setup Serial which is useful for debugging
  // Use the Serial Monitor to view printed messages
  Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");

  dht.setup(DHT_PIN_DATA, DHTesp::DHT22);
  RTC.begin();
  if (!RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  delay(5000);
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 465, 0, 100);
  return percentValue;
}

void printValuesToSerial()
{

  Serial.print("\n\nAnalog Value: ");
  Serial.print(sensorValue);
  Serial.print("\nSoil Moisture : ");
  Serial.print(percent);
  Serial.println("%");
}
void showTime()
{
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void calculateSoilMoister()
{

  analogValue = analogRead(SOILMOISTURE3V3_PIN_SIG);

  // analogVolts = (analogValue * 3.08) / 1024;
  // Serial.print(analogVolts);

  percent = convertToPercent(analogValue);
  printValuesToSerial();
}
void printTempHumidity()
{
  TempAndHumidity measurement = dht.getTempAndHumidity();

  Serial.println("");
  Serial.print("Temp  : ");
  Serial.print(measurement.temperature);
  Serial.println("°C");
  Serial.print("Humid : ");
  Serial.print(measurement.humidity);
  Serial.print("%");
  Serial.println("");
}

void loop()
{
  showTime();
  Serial.println("");
  calculateSoilMoister();
  Serial.println("");
  printTempHumidity();
  Serial.println("");
  Serial.println("===========");

  delay(5000);
}
