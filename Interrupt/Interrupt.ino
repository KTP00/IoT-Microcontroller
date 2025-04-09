int pin;
void setup() {
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2),reverse,CHANGE);
 for(pin = 3;pin<=10;pin++){
pinMode(pin,OUTPUT);  
 }
}

void loop() {
  for(pin=3;pin<=10;pin++){
digitalWrite(pin,HIGH);
delay(500);
digitalWrite(pin,LOW);
delay(500);
  }
}

void reverse(){
    for(pin=10;pin>=3;pin--){
digitalWrite(pin,HIGH);
delay(500);
digitalWrite(pin,LOW);
delay(500);
  }
  
}
