
// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = "";

// Your phone number to send SMS: + (plus sign) and country code, for Portugal +351, followed by phone number
// SMS_TARGET Example for Portugal +351XXXXXXXXX
#define SMS_TARGET  "+66XXXXXXXXX"

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGsmClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
const char* ssid = "TTGO Detail";
const char* password = "123456789";
int count = 0;
unsigned long lasttime = 0;
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define Serial Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT  Serial1

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

const int LDRPIN = A0;
int LDR = 0;
const int SirenPIN = 3;
float z;
bool setPowerBoostKeepOn(int en) {
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en) {
    Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    Wire.write(0x35); // 0x37 is default reg value
  }
  return Wire.endTransmission() == 0;
}
void init800l() {
  // Keep power when running from battery
  Wire.begin(I2C_SDA, I2C_SCL);
  bool isOk = setPowerBoostKeepOn(1);
  Serial.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
}

void initMPU() {
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void initWiFi() {
  Serial.println("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void setup() {
  // Set console baud rate
  Serial.begin(115200);
  pinMode(LDRPIN, INPUT);
  pinMode(SirenPIN, OUTPUT);
  init800l();
  initMPU();
  //initWiFi();
}
void sentsms() {
  String smsMessage = "Sign Damaged";
  if (modem.sendSMS(SMS_TARGET, smsMessage)) {
    Serial.println(smsMessage);
    count = 1;

  }
  else {
    Serial.println("SMS failed to send");
  }
}

void FireAlert() {
  String smsMessage2 = "Control Box Over Heat";
  if (modem.sendSMS(SMS_TARGET, smsMessage2)) {
    Serial.println(smsMessage2);
  }
  else {
    Serial.println("Fire Alert SMS failed to send");
  }
}

void loop() {
  if ((millis() - lasttime) > 500) { //delay 500 ms for processing sensor
  mpu.getEvent(&a, &g, &temp);
   z = a.acceleration.z;
  //float temp = temp.temperature;
  LDR = analogRead(LDRPIN);
  /* Print out the values */
   
     Serial.print("Z: ");
     Serial.print(z);
     Serial.print("\tTemp: ");
     Serial.print(temp.temperature);
     Serial.print("\tLDR: ");
     Serial.println(LDR);     
     lasttime = millis();
  } 
  if (LDR < 10) {
    digitalWrite(SirenPIN, HIGH);
  } else {
    digitalWrite(SirenPIN, LOW);
  }

if (z > 9 || z < -9) {
  // Serial.println("ป้ายล้ม");
  if ((millis() - lasttime) > 1 * 60UL * 60UL * 24UL * 1000UL) {
    count = 0;
    Serial.println("inMillis()");
    lasttime = millis();
  }
  if (count < 1) {
    Serial.println("\nส่งsms");
    sentsms();
  }
}

if (temp.temperature > 70) {
 // FireAlert();
 Serial.println("\nFireAlert");
}

}
