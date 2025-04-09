int sensor = A0;
int buzzer = 3;
int val = 0;
//const byte interruptPin = 2;

/*void blink() {
  digitalWrite(buzzer, HIGH);
delay(200);
digitalWrite(buzzer, LOW);
delay(100);
Serial.println("Detected Water");
}
void receive1()           //ISR function excutes when push button at pinD2 is pressed
{                    

}*/
 
void setup() {
Serial.begin(9600);
pinMode(sensor, INPUT);
pinMode(buzzer, OUTPUT);
}
 
void loop() {
  //attachInterrupt(digitalPinToInterrupt(2),receive1,CHANGE); 
  val = analogRead(sensor);
Serial.println(val);
if (val > 100){
digitalWrite(buzzer, HIGH);
Serial.println("Detected Water");
}
/*if (val > 100){
  attachInterrupt(  digitalPinToInterrupt(interruptPin), blink, CHANGE);
}*/
else{
  digitalWrite(buzzer, LOW);
Serial.println("No Water");
  }

}
