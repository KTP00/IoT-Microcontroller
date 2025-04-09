/*https://github.com/adafruit/DHT-sensor-library
https://github.com/adafruit/Adafruit_Sensor */
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2     
#define DHTTYPE DHT11 
DHT dht(2, DHTTYPE);
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float Humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float Temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float Fahrenheit = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  /* ฟังก์ชั่น isnan เป็นฟังก์ชั่นตรวจสอบว่าค่าของตัวแปรเป็นตัวเลขตัวเลขหรือไม่ โดยจะให้ผลลัพธ์เป็นค่า Integer “1” เมื่อตัวแปรไม่เป็นตัวเลข (Not-a-Number: NaN) และ “0” เมื่อเป็นตัวเลข */
  if (isnan(Humidity) || isnan(Temperature) || isnan(Fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float HeatIndexFahrenheit = dht.computeHeatIndex(Fahrenheit, Humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  float HeatIndexCelsius = dht.computeHeatIndex(Temperature, Humidity, false);

  Serial.print(F("Humidity: "));
  Serial.print(Humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(Temperature);
  Serial.print(F("°C "));
  Serial.print(Fahrenheit);
  Serial.print(F("°F  Heat index: "));
  Serial.print(HeatIndexCelsius);
  Serial.print(F("°C "));
  Serial.print(HeatIndexFahrenheit);
  Serial.println(F("°F"));
}
