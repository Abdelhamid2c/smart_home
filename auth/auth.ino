/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const int Password_Length = 7;
byte data_count = 0;

String Data;

String My_password = "AD123*0";

const int LED = 13;
byte etat ;
bool  porte ;
void setup(){
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    Data += customKey;
    data_count++;
  }

  if (data_count == Password_Length) {
    if (Data == My_password) {
      etat = 1 ;
      
    }
    else {
      etat = 0 ;
    }
    
    clearData();
  }
  
}

void clearData() {
  if(etat == 1){
    digitalWrite(LED, HIGH);
    porte = true;
    Serial.println(" succes password");
  }else{
    porte = false;
    Serial.println("error password");
    digitalWrite(LED, LOW);
  }
  data_count = 0;
  Data ="";
  Serial.println(porte);
}
