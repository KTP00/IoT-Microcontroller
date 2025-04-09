
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

#include <FS.h>

#define DBG_OUTPUT_PORT Serial
const char* ssid = "WiFi_SSID";
const char* password = "WiFi_Password";

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

const char *metaRefreshStr = ""
 "<!DOCTYPE HTML><html>"
 "<head>  <meta name='viewport' content='width=device-width, initial-scale=1'></head>"
"<body><center><h2>ESP8266 Web Server<h2/><br/><img src='54' width='281' height='283'><br/>"
  "<h2>Mr.Thongchai P.<br/> <h3>Advisor<h3/></h2> <br/>"
  "<p>Click URL: <a href='http://rcc.ac.th/dc.php'>RCC.AC.TH</a></p><br/><center/></body></html>"; 

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;


/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1); // note: update metaRefreshStr string if ip change!
IPAddress netMsk(255, 255, 255, 0);

// Web server
ESP8266WebServer server(80);

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

void setup(void){
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
 
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }
  //WIFI INIT
  DBG_OUTPUT_PORT.printf("Connecting to %s\n", ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(myHostname);


  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println ( WiFi.softAPIP() );

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  // Setup MDNS responder
  if (!MDNS.begin(myHostname)) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
  }
  
  
  //SERVER INIT
  //list directory
   server.send(200, "text/html", metaRefreshStr);
//server.on("/54", HTTP_GET, "image/png");
  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}
 
void loop(void){
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}
