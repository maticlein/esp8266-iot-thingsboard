#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define WIFI_AP "RED-WIFI"
#define WIFI_PASSWORD "CONTRASEÑA-WIFI"
#define TOKEN "TOKEN-DISPOSITIVO"

#define DHTPIN 2
#define DHTTYPE DHT11
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

DHT dht(DHTPIN, DHTTYPE);
ThingsBoard tb(wifiClient);
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
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
  
  smartDelay(1000);
  getAndSendTemperatureAndHumidityData();

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

  smartDelay(1000);

  if (gps.location.isValid())
  { 
    Serial.print("Latitud: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" %\t");
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    tb.sendTelemetryFloat("latitude", gps.location.lat());
    tb.sendTelemetryFloat("longitude", gps.location.lng());
  }
  else
  {
    Serial.println(F("INVALID"));
    tb.sendTelemetryFloat("latitude", -38.73336991904116);
    tb.sendTelemetryFloat("longitude", -72.61020036206409);
  }
  
  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);
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

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}