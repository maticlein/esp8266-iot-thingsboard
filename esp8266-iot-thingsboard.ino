#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>

#define WIFI_AP "RED-WIFI"
#define WIFI_PASSWORD "CONTRASEÑA-RED-WIFI"
#define TOKEN "TOKEN-DISPOSITIVO"

#define DHTPIN 2
#define DHTTYPE DHT11

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

DHT dht(DHTPIN, DHTTYPE);
ThingsBoard tb(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  lastSend = 0;
}

void loop()
{
  if ( !tb.connected() ) {
    reconnect();
  }
  if ( millis() - lastSend > 1000 ) {
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }
  tb.loop();
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Obteniendo datos del sensor de temperatura.");

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer los datos del sensor de temperatura!");
    return;
  }

  Serial.println("Enviando datos a ThingsBoard:");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" *C\t");
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" % ");

  Serial.print("Latitud: ");
  Serial.print("-38.4398208486452");
  Serial.print(" %\t");
  Serial.print("Longitud: ");
  Serial.println("-71.88990345808152");

  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);
  tb.sendTelemetryFloat("latitude", -38.4398208486452);
  tb.sendTelemetryFloat("longitude", -71.88990345808152);
}

void InitWiFi()
{
  Serial.println("Conectándose al AP ...");

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado al AP");
}

void reconnect() {
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Conectado al AP");
    }
    Serial.print("Conectándose al nodo de ThingsBoard ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[HECHO]" );
    } else {
      Serial.print( "[FALLIDO]" );
      Serial.println( " : reintentando en 5 segundos]" );
      delay( 5000 );
    }
  }
}