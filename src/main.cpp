#include <DHTesp.h>

// Include Libraries
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
  Serial.println("--------");
  Serial.println("\n\nAnalog Value: ");
  Serial.print(sensorValue);
  Serial.print("\nPercent: ");
  Serial.print(percent);
  Serial.print("%");
  Serial.println("--------");
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

float getSoilMoister()
{

  float res = analogRead(SOILMOISTURE3V3_PIN_SIG);
  analogValue = analogRead(A0);
  Serial.print("analogValue : ");
  Serial.println(analogValue);
  Serial.println("");
  Serial.print("analogVolts : ");
  analogVolts = (analogValue * 3.08) / 1024;
  Serial.print(analogVolts);
  Serial.println("");
  Serial.print("raw som : ");
  Serial.println(res);

  float moisture_percentage = (100.00 - ((analogRead(SOILMOISTURE3V3_PIN_SIG) / 1023.00) * 100.00));
  percent = convertToPercent(analogValue);
  printValuesToSerial();

  return moisture_percentage;
}
// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop()
{
  TempAndHumidity measurement = dht.getTempAndHumidity();
  float moisture_percentage;

  moisture_percentage = getSoilMoister();

  Serial.println("");
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  Serial.println("");
  Serial.print("Temp  : ");
  Serial.println(measurement.temperature);

  Serial.print("Humid : ");
  Serial.println(measurement.humidity);
  Serial.println("====================");
  Serial.println("");

  showTime();
  delay(5000);
}
