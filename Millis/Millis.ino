unsigned long lastTime = 0;
void setup(){
    Serial.begin(115200);

    }

void loop()
{
  if ((millis() - lastTime) > (10*60*1000UL)) {
      Serial.println("Line Sended");
    lastTime = millis();
  }
}
