#define analogPin 0 
float Moisture = 0;
void setup() {
Serial.begin(9600);
}
 
void loop() {
Moisture = analogRead(analogPin); 
Serial.print("Moisture = ");
Serial.println(Moisture);
Serial.println((Moisture*100)/1023);//Convert to percent
if (Moisture < 500) {

}
else {

 
}
delay(100);
}
