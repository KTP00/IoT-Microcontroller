/*
   Arduino Robot Car Wireless Control using the HC-05 Bluetooth
   
                == MASTER DEVICE - Joystick ==
                   
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

int xAxis, yAxis;

void setup() {
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  xAxis = analogRead(A0); // Read Joysticks X-axis
  yAxis = analogRead(A1); // Read Joysticks Y-axis
  
  // Send the values via the serial port to the slave HC-05 Bluetooth device
  Serial.write(analogRead(A0)); // Dividing by 4 for converting from 0 - 1023 to 0 - 256, (1 byte) range
  Serial.write(analogRead(A1));
    Serial.print(xAxis);
    Serial.print("\t");
    Serial.println(yAxis);
  delay(20);
}
