/*https://github.com/adafruit/DHT-sensor-library
https://github.com/adafruit/Adafruit_Sensor */
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);   //Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f //20 column 4 rows
#define DHTPIN 2 //D2    
#define DHTTYPE DHT11 
#define MoisturePinA 5 //A5
#define MoisturePinB 6 //A6
#define MoisturePinC 7 //A7
int joyX = A1;
int SW = 3;
int RelayFlow = 4;
int RelayFog = 5;
int RelayFan = 6;
int dir1PinA = 8;
int dir2PinA = 9;
int speedPinA = 10;
int xValue;
float MoistureA = 0;
float MoistureB = 0;
float MoistureC = 0;
  float Humidity;
  float Temperature;
  int ledgreenA = ;
  int ledredA = ;
  int ledgreenB = ;
  int ledredB = ;
  int ledgreenC = ;
  int ledredC = ;
DHT dht(DHTPIN, DHTTYPE);
unsigned long last_time = 0; //millis()

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(joyX, INPUT);
  pinMode(SW, INPUT);
  pinMode(RelayFlow,OUTPUT);
  pinMode(RelayFog,OUTPUT);
  pinMode(RelayFan,OUTPUT);
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  lcd.begin();
  pinMode(ledgreenA,OUTPUT);
  pinMode(ledredA,OUTPUT);
  pinMode(ledgreenB,OUTPUT);
  pinMode(ledredB,OUTPUT);
  pinMode(ledgreenC,OUTPUT);
  pinMode(ledredC,OUTPUT);
}

void loop() {   
     Temperature_Humidity_Moisture();
     Joystick();
      LCD();
      RelayControl();
      LED();
}


void Temperature_Humidity_Moisture(){
  unsigned long period = 2000; //ระยะเวลาที่ต้องการรอ
  if( millis() - last_time >= period) {
    //lcd.home();
    //lcd.setCursor(0, 1);
    last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
   Humidity = dht.readHumidity();
   Temperature = dht.readTemperature();
   /* Humidity = 49;
   Temperature = 36;*/
  MoistureA = analogRead(MoisturePinA); 
  MoistureA = (100-((MoistureA/1023.00)*100));
  MoistureB = analogRead(MoisturePinB); 
   MoistureB = (100-((MoistureB/1023.00)*100));
  MoistureC = analogRead(MoisturePinC); 
   MoistureC = (100-((MoistureC/1023.00)*100));
  // Check if any reads failed and exit early (to try again).
 /* ฟังก์ชั่น isnan เป็นฟังก์ชั่นตรวจสอบว่าค่าของตัวแปรเป็นตัวเลขตัวเลขหรือไม่ โดยจะให้ผลลัพธ์เป็นค่า Integer “1” เมื่อตัวแปรไม่เป็นตัวเลข (Not-a-Number: NaN) และ “0” เมื่อเป็นตัวเลข */
  if (isnan(Humidity) || isnan(Temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  //float HeatIndexCelsius = dht.computeHeatIndex(Temperature, Humidity, false);
  Serial.print(F("Humidity: "));
  Serial.print(Humidity);

  Serial.print(F("%  Temperature: "));
  Serial.print(Temperature);
 
  Serial.print(F("°C "));
  Serial.print(F("Moisture A : "));
Serial.print(MoistureA);
Serial.print(F("Moisture B : "));
Serial.println(MoistureB);
Serial.print(F("Moisture C : "));
Serial.println(MoistureC);
  //Serial.print(HeatIndexCelsius);
  //Serial.print(F("°C "));
   }
 }

 void Joystick(){
xValue = analogRead(joyX);
//Serial.println(xValue);
if(xValue < 880 && xValue > 10){
  analogWrite(speedPinA, 0);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, LOW);
}
if(xValue < 10){
  analogWrite(speedPinA, 100);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
}
  if(xValue > 880){
  analogWrite(speedPinA, 100);
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
}
}

void LCD(){
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
   lcd.print(Humidity);
   lcd.print(" %");
lcd.setCursor(0, 1);
   lcd.print("Temperature:");
   lcd.print(Temperature);
   lcd.print(" C");
   lcd.setCursor(0, 2);
   lcd.print("Moisture(A) : ");
  lcd.print(MoistureA);
  lcd.setCursor(0, 3);
  lcd.print("(B):");
  lcd.print(MoistureB);
  lcd.print("(C):");
  lcd.print(MoistureC);
 }

viod RelayControl(){
 if(SW == HIGH){
  unsigned long period = 2000; //ระยะเวลาที่ต้องการรอ
  if( millis() - last_time >= period) {
    digitalWrite(RelayFlow,HIGH);
  }
  }
  else if(Temperature > 35){
    digitalWrite(RelayFan,LOW);
   }
  else if(Humidity < 50){
    digitalWrite(RelayFlow,HIGH); 
    digitalWrite(RelayFog,HIGH); 
   }
   else{
    digitalWrite(RelayFlow,LOW);
    digitalWrite(RelayFan,HIGH);
    digitalWrite(RelayFog,LOW);
    }
 }

 void LED(){
    if(MoistureA < 50){
     digitalWrite(ledredA,HIGH);
     digitalWrite(ledgreenA,LOW);
     }else if(MoistureB < 50){
     digitalWrite(ledredB,HIGH);
     digitalWrite(ledgreenB,LOW);
     }else if(MoistureC < 50){
      digitalWrite(ledredC,HIGH);
      digitalWrite(ledgreenC,LOW);
     }else{
      digitalWrite(ledredA,LOW);
      digitalWrite(ledredB,LOW);
      digitalWrite(ledredC,LOW);
      digitalWrite(ledgreenA,HIGH);
      digitalWrite(ledgreenB,HIGH);
      digitalWrite(ledgreenC,HIGH);}
     
 }
 
