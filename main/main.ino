#include <DHT.h>
#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Firebase_ESP_Client.h>
#include "Servo.h"
const int trigPin = 12;
const int echoPin = 14;
#include "Ultrasonic.h"
Ultrasonic ultrasonic(trigPin, echoPin); // Trig et Echo
Servo servo; 

const char* WIFI_SSID     = "DESKTOPAbdelhamid";
const char* WIFI_PASSWORD = "25052002";


String fireHumid = "humidity";
String fireTemp = "temperature"; 

#define API_KEY "AIzaSyB7K7RY5i21RV1j3jsJHkJ2icMqnHe4kX0"
#define DATABASE_URL "https://iot-project-ad019-default-rtdb.firebaseio.com/" 
#define USER_EMAIL "abdelhamidchebel25@gmail.com"
#define USER_PASSWORD "1122334455"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;


unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

DHT dht(2, DHT11);

const byte ventilateur = 13;
const int capteur_humidity_sol = A0;
// const int seuil_humidity = 300;
const int relais = 5;

bool porte;
bool v ;
bool existe = false;
bool vib = false;
bool p ;  
bool ledState;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(ventilateur, OUTPUT); 
  
  pinMode(capteur_humidity_sol,INPUT);
  pinMode(relais,OUTPUT);
  servo.attach(D0);
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);
  pinMode(15,OUTPUT);
  dht.begin();

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  //config.timeout.serverResponse = 10 * 1000;

  digitalWrite(ventilateur,LOW);
  digitalWrite(relais,LOW);
}

void loop() {


  // if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  // {

  // }

    float h = dht.readHumidity();                                
  float t = dht.readTemperature();                              
  
  if (isnan(h) || isnan(t))                                     
  {                                   
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 
  String fireHumid = String(h) + String("%");    
  String fireTemp = String(t) + String("°C");                  //Temperature integer to string conversion
  // delay(1000);

  int hum_sol = analogRead(capteur_humidity_sol);
    //sendDataPrevMillis = millis();  
   if(Firebase.RTDB.getBool(&fbdo, "/led/status", &ledState)){
    if(ledState){
      digitalWrite(15, HIGH);
    }else{
      digitalWrite(15, LOW);
    }
    
   }
   if(Firebase.RTDB.setString(&fbdo, "/dht/temp", fireTemp)){
    Serial.println(fireTemp);
   }
   if(Firebase.RTDB.setString(&fbdo, "/dht/humid", fireHumid)){
    Serial.println(fireHumid);
   }
   
  if(Firebase.RTDB.getBool(&fbdo, "/dht/ventil", &v)){
    if(v){
      digitalWrite(ventilateur,HIGH);
      Serial.println("high");
    }else{
      digitalWrite(ventilateur,LOW);
      Serial.println("low");

    }
   }
    if(v){
      digitalWrite(ventilateur,HIGH);
      Serial.println("high");
    }else{
      digitalWrite(ventilateur,LOW);
      Serial.println("low");

    }
  if(Firebase.RTDB.setInt(&fbdo, "/arrosage/sol", hum_sol)){
    
    if(Firebase.RTDB.getBool(&fbdo, "/arrosage/pompe", &p)){
      if(p){
        digitalWrite(relais, HIGH);
        Serial.println("pompe actif");
      }else{
        digitalWrite(relais, LOW);
        Serial.println("pompe ferme");
      }
   }
    // Serial.println(hum_sol);
   }
  
  
  if(Firebase.RTDB.getBool(&fbdo, "/porte/status", &porte)){
    if(!porte){
      servo.write(0);
       Serial.println("la porte est ferme");
    }else{
      servo.write(150);
      Serial.println("la porte est ouverte") ;
    }
    
   }
  if(digitalRead(D2) ==1){
    digitalWrite(D3,HIGH);
    vib = true;
  }else if (digitalRead(D2) ==0){
    digitalWrite(D3,LOW);
    vib = false ;
  }
  Firebase.RTDB.setBool(&fbdo, "/vibration/status", vib);
    
  int dist = ultrasonic.Ranging(CM);
  Serial.println(dist);
  if(dist < 10){
    // digitalWrite(13,HIGH);
     existe = true;
     Serial.println("chambre non vide");
  }else if (dist>10){
    // digitalWrite(13,LOW);
    existe = false;
    Serial.println("chambre vide");
  }
  

  
   else{
    Serial.println(fbdo.errorReason().c_str());
   }
   Firebase.RTDB.setBool(&fbdo, "/chambre/status", existe);
}
//python -m esptool --chip  esp8266 --port COM8 erase_flash
