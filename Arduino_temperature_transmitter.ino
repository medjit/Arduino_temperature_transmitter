#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_GPIO  5
#define BUZZER_GPIO   A5
#define LED_GPIO      A1
#define RS485_CONTROL_GPIO  2

#define RS485_TRANSMIT  HIGH
#define RS485_RECEIVE   LOW

#define SEND_INTERVAL 10000
unsigned long last_sent = 0;

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
  if((millis() - last_sent) >= SEND_INTERVAL) rs485_tansmit_temp();
  
  digitalWrite(LED_GPIO, !digitalRead(LED_GPIO));
  float temp = get_temperature();
  if (temp == DEVICE_DISCONNECTED_C) {
    digitalWrite(BUZZER_GPIO, HIGH);
    delay(1000);
    digitalWrite(BUZZER_GPIO, LOW);
  }
  rs485_tansmit_temp();
  delay(500);
}

float get_temperature()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void rs485_tansmit_temp()
{
  int temperature = get_temperature();
  digitalWrite(RS485_CONTROL_GPIO, RS485_TRANSMIT);
  Serial.println("T" + (String)temperature);
  digitalWrite(RS485_CONTROL_GPIO, RS485_RECEIVE);
  last_sent = millis();
}
