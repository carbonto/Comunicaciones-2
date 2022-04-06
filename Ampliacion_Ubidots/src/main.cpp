#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
//Definimos las constantes

const char *UBIDOTS_TOKEN = "BBFF-iLJfOM5Chy4UJClrz7QQTCxElIfBAr";
const char *WIFI_SSID = "MIX 2";
const char *WIFI_PASS = "12345678";
const char *DEVICE_LABEL = "ESP32"; //Para publicar los datos
const char *DEVICE_LABEL2 = "owo"; //Para recibir los datos
const char *VARIABLE_LABEL = "ADC6"; //Mandar datos del ADC6
const char *VARIABLE_LABEL2 = "HALL_READ"; //Mandar los datos del sensor efecto Hall
const char *VARIABLE_LABEL3 = "tempc";

const int PUBLISH_FREQUENCY = 5000; //rate in milis

unsigned long timer;
uint8_t analogPin1 = 33; // ADC6
int hall_read = 0; //Sensor de efecto hall

Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL2, VARIABLE_LABEL3); //Se suscribe a la variable temperatura

  timer = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if (abs(millis() - timer) > PUBLISH_FREQUENCY) // Publica cada 5 segundos
  {
     hall_read = hallRead();
    float value = analogRead(analogPin1);
    ubidots.add(VARIABLE_LABEL, value); // Valores a enviar
    ubidots.publish(DEVICE_LABEL);
    
    ubidots.add(VARIABLE_LABEL2, hall_read); // Valores a enviar
    ubidots.publish(DEVICE_LABEL);
    
    ubidots.subscribeLastValue(DEVICE_LABEL2, VARIABLE_LABEL3);
    timer = millis();
  }
  ubidots.loop();
}