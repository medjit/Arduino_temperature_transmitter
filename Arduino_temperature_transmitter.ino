#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_GPIO        5
#define BUZZER_GPIO         A5
#define LED_GPIO            A1
#define HEATER_GPIO         A2
#define RS485_CONTROL_GPIO  2

#define RS485_TRANSMIT      HIGH
#define RS485_RECEIVE       LOW

#define MAX_TEMP            90

#define SEND_INTERVAL       10000
#define TEMP_READ_INTERVAL  5000

unsigned long last_sent = 0;
unsigned long last_read = 0;
boolean err = false;
float temp = 0;

OneWire oneWire(DS18B20_GPIO);
DallasTemperature sensors(&oneWire);

void setup()
{
  pinMode(LED_GPIO, OUTPUT);    digitalWrite(LED_GPIO, LOW);
  pinMode(BUZZER_GPIO, OUTPUT); digitalWrite(BUZZER_GPIO, LOW);
  pinMode(HEATER_GPIO, OUTPUT); digitalWrite(HEATER_GPIO, HIGH);
  
  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(9);
}

void loop()
{
  if((millis() - last_sent) >= SEND_INTERVAL) rs485_tansmit_temp();
  if (millis() - last_read >= TEMP_READ_INTERVAL) {
    temp = get_temperature();
    last_read = millis();
  }
  if (temp == DEVICE_DISCONNECTED_C && !err) {
    digitalWrite(BUZZER_GPIO, HIGH);
    delay(1000);
    digitalWrite(BUZZER_GPIO, LOW);
    delay(1000);
  }
  if(temp >= MAX_TEMP){
    err = true;
    digitalWrite(BUZZER_GPIO, HIGH);
    digitalWrite(HEATER_GPIO, LOW);
  }
  
  digitalWrite(LED_GPIO, HIGH);
  delay(5);
  digitalWrite(LED_GPIO, LOW);
  delay(100);
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
