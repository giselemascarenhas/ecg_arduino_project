#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFISSID ""
#define PASSWORD ""
#define TOKEN ""
#define MQTT_CLIENT_NAME ""

#define VARIABLE_LABEL "ECG_Sensor_Data"
#define DEVICE_LABEL "ECG_Monitoring_System"

#define SENSORPIN A0 
#define LO_PLUS_PIN 10 // GPIO10 (S3)
#define LO_MINUS_PIN 9 // GPIO9 (S2)

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[10000];
char topic[150];

char str_sensor[10];
char str_millis[20];
double epochseconds = 0;
double epochmilliseconds = 0;
double current_millis = 0;
double current_millis_at_sensordata = 0;
double timestampp = 0;
int j = 0;

/****************************************
   Funções Auxiliares
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = '\0';
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Tentando conectar ao MQTT...");

    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

/****************************************
   Funções Principais
 ****************************************/

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  pinMode(SENSORPIN, INPUT);
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);

  Serial.println();
  Serial.print("Aguardando WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
  timeClient.update();
  epochseconds = timeClient.getEpochTime();
  epochmilliseconds = epochseconds * 1000;
  Serial.print("epochmilliseconds=");
  Serial.println(epochmilliseconds);
  current_millis = millis();
  Serial.print("current_millis=");
  Serial.println(current_millis);
}

void loop() {
  if (!client.connected()) {
    reconnect();
    j = 0;
  }

  if (digitalRead(LO_PLUS_PIN) == HIGH || digitalRead(LO_MINUS_PIN) == HIGH) {
    Serial.println("Eletrodos desconectados!");
    delay(1000);
    return;
  }

  j = j + 1;
  Serial.print("j=");
  Serial.println(j);
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\": [", VARIABLE_LABEL);
  for (int i = 1; i <= 3; i++) {
    float sensor = analogRead(SENSORPIN);
    dtostrf(sensor, 4, 2, str_sensor);
    sprintf(payload, "%s{\"value\":", payload);
    sprintf(payload, "%s %s,", payload, str_sensor);
    current_millis_at_sensordata = millis();
    timestampp = epochmilliseconds + (current_millis_at_sensordata - current_millis);
    dtostrf(timestampp, 10, 0, str_millis);
    sprintf(payload, "%s \"timestamp\": %s},", payload, str_millis);
    delay(150);
  }

  float sensor = analogRead(SENSORPIN);
  dtostrf(sensor, 4, 2, str_sensor);
  current_millis_at_sensordata = millis();
  timestampp = epochmilliseconds + (current_millis_at_sensordata - current_millis);
  dtostrf(timestampp, 10, 0, str_millis);
  sprintf(payload, "%s{\"value\":%s, \"timestamp\": %s}]}", payload, str_sensor, str_millis);
  Serial.println("Publicando dados na nuvem Ubidots");
  client.publish(topic, payload);
  Serial.println(payload);
  client.loop();
}
