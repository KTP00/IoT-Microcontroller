//https://l.facebook.com/l.php?u=https%3A%2F%2Ftronixstuff.com%2F2013%2F11%2F19%2Farduino-tutorials-chapter-15-rfid%2F%3Ffbclid%3DIwAR1fz1WQ54_vExunAw7CPqWtvBPZ20ZhvVRJfQXIJRbl0H25oyZ1mUzZ3lc&h=AT3liPh-SPcioiEt36vzNghErE-sNjrWSx3c57g2dVvikFp0lw4Ym24tD8h_AscACIq7_fHfmjOsc49ETlqnB5oPL3lM2Kxz9QypAcip_BpzSZRxxqwahbJKUSzTyFeDSPG-l67ayXtu7YdFrTpkpA
#include <SoftwareSerial.h>
SoftwareSerial RFID(2, 3); // RX and TX

int i;

void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
}

void loop()
{
  if (RFID.available() > 0) 
  {
     i = RFID.read();
     Serial.print(i, DEC);
     Serial.print(" ");
  }
}
