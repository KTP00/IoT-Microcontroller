 //https://randomnerdtutorials.com/display-images-esp32-esp8266-web-server/ 
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "FS.h"

// Replace with your network credentials
const char* ssid = "WiFi_SSID";
const char* password = "WiFi_Password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <center>
  <h2>ESP8266 Web Server<h2/><br/>
  <img src="54" width="281" height="283"><br/>
  <h2>Mr.Thongchai P.<br/> <h3>Advisor<h3/></h2> <br/>
  <p>Click URL: <a href="http://rcc.ac.th/dc.php">RCC.AC.TH</a></p>
  
<br/><center/>
 

</body>  
</html>)rawliteral";


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
    server.on("/54", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/54.png", "image/png");
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
