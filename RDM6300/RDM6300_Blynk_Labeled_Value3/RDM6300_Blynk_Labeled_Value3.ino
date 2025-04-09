

//http://help.blynk.cc/getting-started-library-auth-token-code-examples/blynk-basics/how-to-display-any-sensor-data-in-blynk-app?fbclid=IwAR1WeJiWFYNJV7M1BnGbnjR60XESob9nE69yrya3QPdt0thmrrOt3jUeIu0

#include <SoftwareSerial.h>
SoftwareSerial RFID(D3, D4); // RX and TX
#include "HardwareSerial.h"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "811e8ac094d0442c98b962f1316d9fad";
char ssid[] = "11435";
char pass[] = "11111111";
#define PIN_UPTIME V5


int i;
String str;
//int a;
String i3 = "2 3 15 656 595";
BLYNK_READ(PIN_UPTIME)
{
  
Blynk.virtualWrite(PIN_UPTIME,str);//(V5,ค่า)  //Sent to App
  
}BLYNK_READ(V4)
{
Blynk.virtualWrite(V4,i3);//(V5,ค่า)  //Sent to App
}


void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  Blynk.begin(auth, ssid, pass);
}
int count=0;
void loop()
{
 
  if(RFID.available() > 0) 
  {
   
    i = RFID.read();
   str += i;
   str +=" ";
   count++;
  
  }
  
  switch(count){
    
    default : Serial.print(str);
              str="";
              break;
    
  }
  
   Blynk.run();
   
 
}

