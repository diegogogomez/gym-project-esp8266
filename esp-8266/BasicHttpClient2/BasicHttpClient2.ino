/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

bool stringComplete = false;  // whether the string is complete
String inputString = "";         // a String to hold incoming data

void setup() {

  inputString.reserve(200);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("MARIA FLOREZ 2.4", "51991665");

}

void loop() {
  if (stringComplete) {

    while(1){
      if ((WiFiMulti.run() == WL_CONNECTED)) {

        Serial.printf("Estado de la conexión: %d\n", WiFi.status());
        Serial.printf("aprobado");
        WiFiClient client;
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        if (http.begin(client, "http://backend-gym-app.herokuapp.com/api/usuario/verificar")) {  // HTTP
  
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.print(payload);
            Serial.println(inputString);
            Serial.println("Z");
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        break;
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
        
      }
      
    }
    
    inputString = "";
    stringComplete = false;

  // Serial.printf("Estado de la conexión: %d\n", WiFi.status());
  // WiFi.printDiag(Serial);
  // delay(2000);


  
  // wait for WiFi connection

/*
 * 
 *     if ((WiFiMulti.run() == WL_CONNECTED)) {
  
      WiFiClient client;
  
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://backend-gym-app.herokuapp.com/api/usuario/verificar")) {  // HTTP
  
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    }
    stringComplete = false;
  }
 * 
*/


  // delay(10000);
  }
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
