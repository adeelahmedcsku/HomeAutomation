

/*************************************************************
IoT PROJECT
-----------
NAME - Raza Akbar and Adeel Adeel

SUBMITTED TO
------------
Prof. MAURIZIO PALESI

*************************************************************/
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLJBtLXUPi"
#define BLYNK_DEVICE_NAME "home autoamtion"
#define BLYNK_AUTH_TOKEN "AFgRZQCJDoGO-91ur-sc5K1lTjhraskN"

// These are the constants we define for our project
#define ac_idicator 8
#define heaterIndicator 12
#define roomLights 3

#define BLYNK_PRINT Serial

//The Yún has two processors on board. One is an ATmega32U4 like on the Leonardo. The other is an Atheros 9331, 
//running Linux and the OpenWRT wireless stack, which enables the board to connect to WiFi and Ethernet networks.
#include <Bridge.h>
#include <BlynkSimpleYun.h>
// Adding the LCD dispplay libraray
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 13, en = 10;
LiquidCrystal lcd(13, 10, 7, 6, 5, 4);

char auth[] = BLYNK_AUTH_TOKEN;

float temp;
int tempPin = A0;
float lightVal;
int lightPin = A1;
bool automate = 0;
int roomLightsButton=0;
void setup(){
    
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    Blynk.begin(auth);

    pinMode(8, OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(3,OUTPUT);
    turnOffAutomation();
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  
}

// switch for on/off the circuit  or main button
BLYNK_WRITE(V7){
    int value = param.asInt();
    if (value == 1){
        automate = 1;
         lcd.setCursor(0, 0);
        lcd.print("Home Automation");
    }
    else{
        automate = 0;
        roomLightsButton=0;
        lcd.clear();        
       
    }
}

BLYNK_WRITE(V3){
   int value = param.asInt();
  roomLightsButton = value;
    if (value == 1){
         //automate = 1;
        Blynk.virtualWrite(V0, HIGH);
       digitalWrite(roomLights, HIGH);
        Blynk.virtualWrite(V2, LOW);
        roomLightsButton=1;
        Blynk.virtualWrite(V2, 0);
        

    }
    else{
       //automate = 0;  
        Blynk.virtualWrite(V0, LOW);
        Blynk.virtualWrite(V2, HIGH);
       digitalWrite(roomLights, LOW);
       roomLightsButton=0;
        

    }
}


BLYNK_CONNECTED(){
    Blynk.syncVirtual(V0);
    Blynk.syncVirtual(V1);
    Blynk.syncVirtual(V2);
    Blynk.syncVirtual(V3);
    Blynk.syncVirtual(V4);
    Blynk.syncVirtual(V5);
    Blynk.syncVirtual(V6);
    Blynk.syncVirtual(V7);
}

void temperature_fun(){
    // We are using the same statement two times just for removing -
    //the ambiguous value when connecting both temp sensor and light sensor
    temp = analogRead(tempPin);
    temp = analogRead(tempPin); 

    // read analog volt from sensor and save to variable temp
    temp = temp * 0.48828125;
    // convert the analog volt to its temperature equivalent
    Serial.print("TEMPERATURE = ");
    Serial.print(temp); // display temperature value
    Serial.print("*C");
    Serial.println();
   // lcd.print("TEMP=");
   // lcd.print("TEMP=");
    Blynk.virtualWrite(V1, temp);
  // LCD Work start here
  lcd.setCursor(0, 1);
  lcd.print("Temp=");
  // print the number of seconds since reset:
  lcd.setCursor(5, 1);
  lcd.print((int)temp);

  lcd.setCursor(7, 1);
  lcd.print(" *C");
  
    if (temp > 30){
        digitalWrite(ac_idicator,HIGH); //AC ON
        Blynk.virtualWrite(V4, HIGH);
    }
    else{
        digitalWrite(ac_idicator,LOW);  //AC OFF
        Blynk.virtualWrite(V4, LOW);
    } 
    
    if (temp < 25){
        digitalWrite(heaterIndicator,HIGH); //Heater ON
        Blynk.virtualWrite(V6, HIGH);
    }
    else{
        digitalWrite(heaterIndicator,LOW); //Heater OFF
        Blynk.virtualWrite(V6, LOW);
    } 
   // delay(1000); // update sensor reading each one second
}


void light_fun(){
    //digitalWrite(roomLights, LOW);   // turn the LED on (HIGH is the voltage level)
    // We are using the same statement two times just for removing -
    //the ambiguous value when connecting both temp sensor and light sensor
    if(roomLightsButton==1)
    {
   
    }
    else
    {
       lightVal = analogRead(lightPin);
    lightVal = analogRead(lightPin);
    Serial.print(lightVal);
    if(lightVal < 30)
    {
        digitalWrite(roomLights,HIGH); // Room lights
        Blynk.virtualWrite(V0, HIGH);
        Blynk.virtualWrite(V2, lightVal);
     }
     else
     {
       digitalWrite(roomLights,LOW); //Room lights
        Blynk.virtualWrite(V0, LOW);
        Blynk.virtualWrite(V2, lightVal);
     }
    //Blynk.virtualWrite(V2, lightVal);

    Serial.print("LIGHT INTENSITY = ");
    Serial.print(lightVal); 
    Serial.println();
    }
}


void turnOffAutomation(){
    //digitalWrite(roomLights, LOW);   // turn the LED on (HIGH is the voltage level)
    // We are using the same statement two times just for removing -
    //the ambiguous value when connecting both temp sensor and light sensor
   Blynk.virtualWrite(V7, LOW);
    Blynk.virtualWrite(V6, LOW);
    Blynk.virtualWrite(V5, LOW);
    Blynk.virtualWrite(V4, LOW);
    Blynk.virtualWrite(V3, LOW);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V1, LOW);
     Blynk.virtualWrite(V0, LOW);
    digitalWrite(ac_idicator, LOW);
    digitalWrite(heaterIndicator, LOW);
    //lcd.clear();

}

void loop(){
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
 
    Blynk.run();
    if (automate == 1){
        //lcd.print("Home Automation");
        temperature_fun();
        light_fun();

    }
    else{
        turnOffAutomation();
        //lcd.clear();
    }

}
