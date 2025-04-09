int pushButton = 2;
unsigned long period = 50;
unsigned long lasttime = 0;
void setup() {
  Serial.begin(9600);
  pinMode(pushButton, INPUT);
}
void loop()
{
  if ((millis() - lasttime) >= period) {
    int buttonState = digitalRead(pushButton);
    Serial.println(buttonState);
    lasttime = millis();
  }
}
