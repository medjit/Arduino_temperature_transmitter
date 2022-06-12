#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_GPIO  5
#define BUZZER_GPIO   A5
#define LED_GPIO      A1

OneWire oneWire(DS18B20_GPIO);
DallasTemperature sensors(&oneWire);

void setup()
{
  pinMode(LED_GPIO, OUTPUT);
  pinMode(BUZZER_GPIO, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(9);
}

void loop()
{
  digitalWrite(LED_GPIO, !digitalRead(LED_GPIO));
  float temp = getTemperature();
  if (temp == DEVICE_DISCONNECTED_C) {
    digitalWrite(BUZZER_GPIO, HIGH);
    delay(1000);
    digitalWrite(BUZZER_GPIO, LOW);
  }
  Serial.println("Temperature for the device 1 (index 0) is: " + (String)temp);
  delay(500);
}

float getTemperature()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  return tempC;
}
