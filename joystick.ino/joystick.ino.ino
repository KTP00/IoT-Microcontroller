int joyX = A0;
int joyY = A1;
 int xValue;
  int yValue;
void setup() {
  pinMode (joyX, INPUT) ;                     
  pinMode (joyY, INPUT) ;
  Serial.begin(9600);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
 
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
}
