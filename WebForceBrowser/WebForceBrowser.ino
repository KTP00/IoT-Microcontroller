#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

/*String responseHTML = ""
                      "<!DOCTYPE html><html lang='en'><head>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example."
                      " All requests will be redirected here.</p></body></html>";*/    

String responseHTML = ""
 "<!DOCTYPE HTML><html>"
 "<head>  <meta name='viewport' content='width=device-width, initial-scale=1'></head>"
"<body><center><h2>ESP8266 Web Server<h2/><br/><img src='54' width='281' height='283'><br/>"
  "<h2>Mr.Thongchai P.<br/> <h3>Advisor<h3/></h2> <br/>"
  "<p>Click URL: <a href='http://rcc.ac.th/dc.php'>RCC.AC.TH</a></p><br/><center/></body></html>";                 
                                  

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Esp8266","");
Serial.println(WiFi.localIP());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.serveStatic("/", SPIFFS, "/index.html");
  webServer.serveStatic("/54.pngpng", SPIFFS, "/54.jpg");

  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
