/*https://github.com/adafruit/DHT-sensor-library
  https://github.com/adafruit/Adafruit_Sensor */
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);   //Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f //20 column 4 rows

#define DHTPIN 8 //D8    
#define DHTTYPE DHT11

#define MoisturePin1 2 //A2-A7
#define MoisturePin2 3
#define MoisturePin3 4
#define MoisturePin4 5
#define MoisturePin5 6
#define MoisturePin6 7

#define RelayPump 4
#define RelayFog 5
#define RelayFan 6
#define RelayValve1 30
#define RelayValve2 31
#define RelayValve3 32
#define RelayValve4 33
#define RelayValve5 34
#define RelayValve6 35

#define SWpin1 2 //D2-3,D18-21
#define SWpin2 3
#define SWpin3 18
#define SWpin4 19
#define SWpin5 20
#define SWpin6 21

#define LED1 36 //D36-41
#define LED2 37
#define LED3 38
#define LED4 39
#define LED5 40
#define LED6 41

//เก็บค่าความชื้นในดิน
float MoistureA = 0;
float MoistureB = 0;
float MoistureC = 0;
float MoistureD = 0;
float MoistureE = 0;
float MoistureF = 0;
int SW1,SW2,SW3,SW4,SW5,SW6;
//ความชื้นในอากาศ
float Humidity;
//อุณหภูมิ
float Temperature;
DHT dht(DHTPIN, DHTTYPE);
unsigned long last_time = 0; //millis()

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  pinMode(RelayPump, OUTPUT);
  pinMode(RelayFog, OUTPUT);
  pinMode(RelayFan, OUTPUT);
  pinMode(RelayValve1, OUTPUT);
  pinMode(RelayValve2, OUTPUT);
  pinMode(RelayValve3, OUTPUT);
  pinMode(RelayValve4, OUTPUT);
  pinMode(RelayValve5, OUTPUT);
  pinMode(RelayValve6, OUTPUT);

  pinMode(SWpin1, INPUT);
  pinMode(SWpin2, INPUT);
  pinMode(SWpin3, INPUT);
  pinMode(SWpin4, INPUT);
  pinMode(SWpin5, INPUT);
  pinMode(SWpin6, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  attachInterrupt(SWpin1, RelayControl, RISING);//RISING is LOW to HIGH
  attachInterrupt(SWpin2, RelayControl, RISING);
  attachInterrupt(SWpin3, RelayControl, RISING);
  attachInterrupt(SWpin4, RelayControl, RISING);
  attachInterrupt(SWpin5, RelayControl, RISING);
  attachInterrupt(SWpin6, RelayControl, RISING);
}

void loop() {
  SW1 = digitalRead(SWpin1);
  SW2 = digitalRead(SWpin2);
  SW3 = digitalRead(SWpin3);
  SW4 = digitalRead(SWpin4);
  SW5 = digitalRead(SWpin5);
  SW6 = digitalRead(SWpin6);
  Temperature_Humidity_Moisture();
  if ( millis() - last_time >= 1000UL) {
  LCD();
  Automation();
  LED();}
}


void Temperature_Humidity_Moisture() {

  unsigned long period = 2000; //ระยะเวลาที่ต้องการรอ
  if ( millis() - last_time >= period) {
    //lcd.home();
    //lcd.setCursor(0, 1);
    last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    Humidity = dht.readHumidity();
    Temperature = dht.readTemperature();
    /* Humidity = 49;
      Temperature = 36;*/
    MoistureA = analogRead(MoisturePin1);
    MoistureA = (100 - ((MoistureA / 1023.00) * 100));
    MoistureB = analogRead(MoisturePin2);
    MoistureB = (100 - ((MoistureB / 1023.00) * 100));
    MoistureC = analogRead(MoisturePin3);
    MoistureC = (100 - ((MoistureC / 1023.00) * 100));
    MoistureD = analogRead(MoisturePin4);
    MoistureD = (100 - ((MoistureD / 1023.00) * 100));
    MoistureE = analogRead(MoisturePin5);
    MoistureE = (100 - ((MoistureE / 1023.00) * 100));
    MoistureF = analogRead(MoisturePin6);
    MoistureF = (100 - ((MoistureF / 1023.00) * 100));
    // Check if any reads failed and exit early (to try again).
    /* ฟังก์ชั่น isnan เป็นฟังก์ชั่นตรวจสอบว่าค่าของตัวแปรเป็นตัวเลขตัวเลขหรือไม่ โดยจะให้ผลลัพธ์เป็นค่า Integer “1” เมื่อตัวแปรไม่เป็นตัวเลข (Not-a-Number: NaN) และ “0” เมื่อเป็นตัวเลข */
    if (isnan(Humidity) || isnan(Temperature)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    //float HeatIndexCelsius = dht.computeHeatIndex(Temperature, Humidity, false);
    Serial.print(F("Humidity: "));
    Serial.print(Humidity);

    Serial.print(F("%  Temperature: "));
    Serial.print(Temperature);
    Serial.print(F("°C "));

    Serial.print(F("Moisture A : "));
    Serial.print(MoistureA);
    Serial.print(F("Moisture B : "));
    Serial.println(MoistureB);
    Serial.print(F("Moisture C : "));
    Serial.println(MoistureC);
    Serial.print(F("Moisture D : "));
    Serial.print(MoistureD);
    Serial.print(F("Moisture E : "));
    Serial.println(MoistureE);
    Serial.print(F("Moisture F : "));
    Serial.println(MoistureF);
    //Serial.print(HeatIndexCelsius);
    //Serial.print(F("°C "));
  }
}

void LCD() {
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(Humidity);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.print(Temperature);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print("Moisture(A) : ");
  lcd.print(MoistureA);
  lcd.setCursor(0, 3);
  lcd.print("(B):");
  lcd.print(MoistureB);
  lcd.print("(C):");
  lcd.print(MoistureC);
}
void Automation() {

  if (Temperature > 35) {
    digitalWrite(RelayFan, HIGH);
  }
  if (Humidity < 50) {
    digitalWrite(RelayFog, HIGH);
  }
  else {
    digitalWrite(RelayPump, LOW);
    digitalWrite(RelayFan, LOW);
    digitalWrite(RelayFog, LOW);
  }
}

void RelayControl() {
  Serial.println("IN Void RelayControl");
  if (SW1 == HIGH) {
    digitalWrite(RelayValve1,HIGH);
  }else{digitalWrite(RelayValve1,LOW);}
  if (SW2 == HIGH) {
    digitalWrite(RelayValve2,HIGH);
  }else{digitalWrite(RelayValve2,LOW);}
  if (SW3 == HIGH) {
    digitalWrite(RelayValve3,HIGH);
  }else{digitalWrite(RelayValve3,LOW);}
  if (SW4 == HIGH) {
    digitalWrite(RelayValve4,HIGH);
  }else{digitalWrite(RelayValve4,LOW);}
  if (SW5 == HIGH) {
    digitalWrite(RelayValve5,HIGH);
  }else{digitalWrite(RelayValve5,LOW);}
  if (SW6 == HIGH) {
    digitalWrite(RelayValve6,HIGH);
  }else{digitalWrite(RelayValve6,LOW);}
}

void LED() {
  if (MoistureA < 50) {
    digitalWrite(LED1, HIGH);

  } else if (MoistureB < 50) {
    digitalWrite(LED2, HIGH);

  } else if (MoistureC < 50) {
    digitalWrite(LED3, HIGH);

  } else if (MoistureD < 50) {
    digitalWrite(LED4, HIGH);

  } else if (MoistureE < 50) {
    digitalWrite(LED5, HIGH);

  } else if (MoistureF < 50) {
    digitalWrite(LED6, HIGH);

  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
  }
}
