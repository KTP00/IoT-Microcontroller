//switch press to open press to close
#define swpin  2
#define ledpin 3
boolean btnstatus,status = LOW,laststatus;
void setup()
{ 
  pinMode(ledpin , OUTPUT);
  pinMode(swpin, INPUT);
  Serial.begin(115200);
}

void loop()
{
  btnstatus = digitalRead(swpin);
    Serial.println(btnstatus);
  
  if((btnstatus == HIGH && laststatus == LOW)) status = !status;
    digitalWrite(ledpin,status);
    laststatus = btnstatus; 
    delay(50);
}
