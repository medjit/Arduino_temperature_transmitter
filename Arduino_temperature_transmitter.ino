#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_GPIO  5

OneWire oneWire(DS18B20_GPIO);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(9);
}

void loop()
{
  Serial.println("Temperature for the device 1 (index 0) is: " + (String)getTemperature());
  delay(500);
}

float getTemperature()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  return tempC;
}
