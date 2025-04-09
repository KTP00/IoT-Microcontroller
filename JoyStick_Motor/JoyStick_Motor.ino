int joyX = A1;
int SW = 3;
int dir1PinA = 8;
int dir2PinA = 9;
int speedPinA = 10; //   เพื่อให้ PWM สามารถควบคุมความเร็วมอเตอร์ 
int xValue;
void setup()
{
  Serial.begin(9600);
  pinMode(joyX, INPUT);
  pinMode(SW, INPUT);
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
}

void loop()
{
xValue = analogRead(joyX);
Serial.println(xValue);
if(xValue < 700 && xValue > 200){
  analogWrite(speedPinA, 0);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, LOW);
}
if(xValue < 200){
  analogWrite(speedPinA, 100);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
}
  if(xValue > 800){
  analogWrite(speedPinA, 100);
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
}
}
