#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Firebase_ESP_Client.h>


#ifndef APSSID
#define APSSID "DESKTOPAbdelhamid"
#define APPSK "25052002"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

#define API_KEY "AIzaSyB7K7RY5i21RV1j3jsJHkJ2icMqnHe4kX0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://iot-project-ad019-default-rtdb.firebaseio.com/" 

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "abdelhamidchebel25@gmail.com"
#define USER_PASSWORD "1122334455"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);       // turn LED off by default

  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
   config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}

void loop() {
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();  
  int ledState;
   if(Firebase.RTDB.getInt(&fbdo, "/led/state", &ledState)){
    digitalWrite(LED_BUILTIN, ledState);
   }
   String fireTemp = "123";
   if(Firebase.RTDB.setString(&fbdo, "/hello/temp", fireTemp)){
    Serial.println(fireTemp);
   }
   if(Firebase.RTDB.setString(&fbdo, "/dht/humid", &fireHumid)){
    Serial.println(fireHumid);
   }
   else{
    Serial.println(fbdo.errorReason().c_str());
   }
  }
}
