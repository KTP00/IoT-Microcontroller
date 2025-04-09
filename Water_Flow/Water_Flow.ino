#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
volatile double waterFlow;
float Liter = 1000;
int pin = D2;
char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "";
char pass[] = "";
void setup() {
 Serial.begin(9600);
 waterFlow = 0;
 attachInterrupt(pin, pulse, RISING); //DIGITAL Pin 2: Interrupt 0
 Blynk.begin(auth, ssid, pass);
}
void loop() {
   Blynk.run();
 Serial.print("waterFlow:");
 Serial.print(waterFlow);
 Serial.println(" L");
 // waterFlow = 0;
 delay(500);

}

void pulse() //measure the quantity of square wave
{
 waterFlow += 1.0 / 5880.0;
 //Liter -= 0.0001;
 Liter -= 1.0 / 5880.0;

}
BLYNK_READ(V1) 
{
  Blynk.virtualWrite(V1, Liter);
 // Blynk.virtualWrite(V1, waterFlow);
}
