#include <FS.h>
#include <TridentTD_LineNotify.h>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include "SPIFFS.h"
#include <Wire.h>
// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb
#include <TinyGsmClient.h>
const char* ssid = "WiFi_SSID";
const char* password = "WiFi_Password";
const char simPIN[]   = "";
#define SMS_TARGET  "+66XXXXXXXXX"
#define Serial Serial
#define SerialAT  Serial1
#define LINE_TOKEN  "qLO2AoicGdX8fgIAGsqiamKgzTfaVPh2y0zryZCtQ"
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00
// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

TaskHandle_t Task1;
TaskHandle_t Task2;
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
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;
int count1 = 0;
// Create a sensor object
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

//Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;

void initsms() {
  Serial.begin(115200);
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
  //modem.restart();
  modem.init();
  // use modem.init() if you don't need the complete restart
  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  // To send an SMS, call modem.sendSMS(SMS_TARGET, smsMessage)
  /*String smsMessage = "Hello Esp32";
    if(modem.sendSMS(SMS_TARGET, smsMessage)){
    Serial.println(smsMessage);
    }
    else{
    Serial.println("SMS failed to send");
    }*/
}
// Init MPU6050
void initMPU() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);
  //else{Serial.println("Disconnect");}

}

String getGyroReadings() {
  mpu.getEvent(&a, &g, &temp);

  float gyroX_temp = g.gyro.x;
  if (abs(gyroX_temp) > gyroXerror)  {
    gyroX += gyroX_temp / 50.00;
  }

  float gyroY_temp = g.gyro.y;
  if (abs(gyroY_temp) > gyroYerror) {
    gyroY += gyroY_temp / 70.00;
  }

  float gyroZ_temp = g.gyro.z;
  if (abs(gyroZ_temp) > gyroZerror) {
    gyroZ += gyroZ_temp / 90.00;
  }
  readings["gyroX"] = String(accX);
  readings["gyroY"] = String(accY);
  readings["gyroZ"] = String(accZ);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

String getAccReadings() {
  mpu.getEvent(&a, &g, &temp);
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  //Sent to Web Json key:Value
  readings["accX"] = String(gyroX);
  readings["accY"] = String(gyroY);
  readings["accZ"] = String(gyroZ);
  String accString = JSON.stringify (readings);
  return accString;
}

String getTemperature() {
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  return String(temperature);
}
void initweb() {
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    gyroX = 0;
    gyroY = 0;
    gyroZ = 0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest * request) {
    gyroX = 0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest * request) {
    gyroY = 0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest * request) {
    gyroZ = 0;
    request->send(200, "text/plain", "OK");
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
}





void setup() {
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();
  initsms();
  initMPU();
  initweb();
  xTaskCreatePinnedToCore(
    sentWeb,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    mainProgram,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  
}
void sentWeb(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    if ((millis() - lastTime) > gyroDelay) {
      // Send Events to the Web Server with the Sensor Readings
      events.send(getGyroReadings().c_str(), "gyro_readings", millis());
      lastTime = millis();
    }
    if ((millis() - lastTimeAcc) > accelerometerDelay) {
      // Send Events to the Web Server with the Sensor Readings
      events.send(getAccReadings().c_str(), "accelerometer_readings", millis());
      lastTimeAcc = millis();
    }
    if ((millis() - lastTimeTemperature) > temperatureDelay) {
      // Send Events to the Web Server with the Sensor Readings
      events.send(getTemperature().c_str(), "temperature_reading", millis());
      lastTimeTemperature = millis();
    }
    // delay(1000);
  }
}
void sentsms() {
  String smsMessage = "ป้ายเกิดความเสียหาย";
  if (modem.sendSMS(SMS_TARGET, smsMessage)) {
    Serial.println(smsMessage);
  } else {
    Serial.println("SMS failed to send");
  }
}
void mainProgram(void *pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    while (accZ > 8 || accZ < -6) {

      if ((millis() - lastTime) > 1 * 60UL * 1000UL) {
        count1 = 0;
        Serial.println("inmilis()");
        lastTime = millis();
      }
      //delay sent line notify
      if (count1 < 1) {
        if (WiFi.status() == WL_CONNECTED) {
          LINE.notify("ป้ายล้มแล้วจร้าาา");
        }
        Serial.print("\nป้ายล้มแล้วจร้าาา");
        sentsms();

        count1 = 1;
      }
    }
    //delay(700);
  }
}

void loop() {
  delay(1);
}
