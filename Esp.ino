/*
* Copyright (C) 2022 by Roberto Calvo-Palomino
*
*
*  This programa is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with RTL-Spec.  If not, see <http://www.gnu.org/licenses/>.
*
*   Authors: Roberto Calvo-Palomino <roberto.calvo [at] urjc [dot] es>
*/

// Subject: Sistemas Empotrados y de Tiempo Real
// Universidad Rey Juan Carlos, Spain

#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>

#define RXD2 33
#define TXD2 4

//const char* ssid = "wifieif2";
const char* ssid = "eduroam";
#define EAP_ANONYMOUS_IDENTITY "20220719anonymous@urjc.es" // leave as it is
#define EAP_IDENTITY ""    // Use your URJC email
#define EAP_PASSWORD ""            // User your URJC password
#define EAP_USERNAME ""   // Use your URJC email

#define START_LAP '0'
#define END_LAP '1'
#define OBSTACLE_DETECTED '2'
#define LINE_LOST '3'
#define PING '4'
#define INIT_LINE_SEARCH '5'
#define STOP_LINE_SEARCH '6'
#define LINE_FOUND '7' 
#define VISIBLE_LINE '8'


char server[50] = "193.147.53.2";
int port = 21883;

#define MQTT_USERNAME "VillaBorderline"
#define MQTT_PASSWORD "8"

char mensajeRecibido[256];

// Configuración del cliente MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, server, port, MQTT_USERNAME, MQTT_PASSWORD);

// Configuración del feed MQTT
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, "/SETR/2023/8/");

void initWiFi() {

  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.disconnect(true);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 


  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  initWiFi();


  // Inicia la conexión MQTT
  connectToMQTT();
    // Verifica la conexión MQTT
  if (!mqtt.connected()) {
    connectToMQTT();
  }
}

String jsonString;
String sendBuff;
bool second_read = false;
char last_read;

void loop() {

  if (Serial2.available()) {

    char c = Serial2.read();
    // segun lo que le llegaue manda un json
    if (START_LAP == c) {
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"START_LAP\" \n}";
      sendMQTTMessage(jsonString.c_str());
    } else if (END_LAP == c) {
      sendBuff = Serial2.readStringUntil('\n');
      jsonString = "{\n\t\"team_name\" : \"VillaBorderline\"\n\t \"id\" : \"8\" \n\t \"action\" : \"END_LAP\" \n\t\"time\": " + sendBuff + "\n}";
      sendMQTTMessage(jsonString.c_str());
      sendBuff = "";
    } else if (OBSTACLE_DETECTED == c) {
      sendBuff = Serial2.readStringUntil('\n');
      jsonString = "{\n\t\"team_name\" : \"VillaBorderline\"\n\t \"id\" : \"8\" \n\t \"action\" : \"OBSTACLE_DETECTED\" \n\t\"distance\": " + sendBuff + "\n}";
      sendMQTTMessage(jsonString.c_str());
      sendBuff = "";
    } else if (LINE_LOST == c) {
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"LINE_LOST\" \n}";
      sendMQTTMessage(jsonString.c_str());
    } else if (PING == c) {
      sendBuff = Serial2.readStringUntil('\n');
      jsonString = "{\n\t\"team_name\" : \"VillaBorderline\"\n\t \"id\" : \"8\" \n\t \"action\" : \"PING\" \n\t\"time\": " + sendBuff + "\n}";
      sendMQTTMessage(jsonString.c_str());
      sendBuff = "";
    } else if (INIT_LINE_SEARCH == c) {
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"INIT_LINE_SEARCH\" \n}";
      sendMQTTMessage(jsonString.c_str());
    } else if (STOP_LINE_SEARCH == c) { 
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"STOP_LINE_SEARCH\" \n}";
      sendMQTTMessage(jsonString.c_str());
    } else if (LINE_FOUND == c) { 
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"LINE_FOUND\" \n}";
      sendMQTTMessage(jsonString.c_str());
    } else if (VISIBLE_LINE == c) {
      sendBuff = Serial2.readStringUntil('\n');
      jsonString = "{\n\t\"team_name\": \"VillaBorderline\",\n\t \"id\": \"8\" ,\n\t \"action\": \"VISIBLE_LINE\" \n\t\"value\": " + sendBuff + "\n}";
      sendMQTTMessage(jsonString.c_str());
    } 
  }
}

// Función para conectar al servidor MQTT
void connectToMQTT() {
  Serial.println("Conectando al servidor MQTT...");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Reintentando la conexión MQTT en 5 segundos...");
    delay(100);
  }
  // Al conectarse al wifi envia eso
  Serial2.write("Conectado al servidor MQTT");
  Serial.println("Conectado al servidor MQTT");
}

// Función para enviar mensajes al servidor MQTT
void sendMQTTMessage(const char* message) {
  if (feed.publish(message)) {
    Serial.println("Mensaje enviado con éxito");
  } else {
    Serial.println("Fallo al enviar el mensaje");
  }
}