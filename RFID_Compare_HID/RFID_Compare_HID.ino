#include <ESP8266WiFi.h>

#include <TridentTD_LineNotify.h>
#define RDM6300_PACKET_SIZE  14 //tag card ขนาด 14
#define RDM6300_PACKET_BEGIN 0x02 //index แรกเลข 2 
#define RDM6300_PACKET_END   0x03 //index แรกเลข 3 
#define LINE_TOKEN  "YOUR_TOKEN"
// Globack for reading RFID data
char buff[RDM6300_PACKET_SIZE];//ประกาศตัวแปรประเภทchar buff ขนาด 14
uint32_t tag_id;
uint8_t checksum;
uint32_t Str ;
unsigned long last_time = 0;
int zero = 0;//เก็บเลขศูนย์ไว้ต่อในเลข HIP                  //Index
String dataset[5][2] = {{"0002676051", "user1"},   //{"0,0","0,1"}
  {"0002019241", "user6"},                         //{"1,0","1,1"}
  {"0002671236", "user3"},                         //{"2,0","2,1"}
  {"1234", "user4"},                               //{"3,0","3,1"}
  {"1235", "user5"},
};  //{"4,0","4,1"}
const char* ssid     = "vivo 1901";
const char* password = "12345678";

boolean CompareTag(String compare, String newtag) {
  boolean check = false;
  if (compare == newtag) {
    check = true;
  }
  return check;
}

void ConvertToHid() {
  while (Serial.available() > 0) {
    if (Serial.peek() == RDM6300_PACKET_BEGIN || Serial.read()) {
      Serial.readBytes(buff, RDM6300_PACKET_SIZE);
      if (buff[13] != RDM6300_PACKET_END) {
        Serial.println("Bad1"); //Packet Lost
      }
      //ถ้าเลขที่รับมาครบให้ convert to HID
      else {
        buff[13] = 0;
        checksum = strtol(buff + 11, NULL, 16);
        buff[11] = 0;
        tag_id = strtol(buff + 3, NULL, 16);
        buff[3] = 0;
        checksum ^= strtol(buff + 1, NULL, 16);
        for (uint8_t i = 0; i < 32; i += 8) checksum ^= ((tag_id >> i) & 0xFF);
        if (checksum) {
          Serial.println("Bad2");
        }
        else {
          String Str = String(tag_id);
          for (; Str.length() < 10;)
            // for(;Str.length()<10;)
            Str = String(zero) + Str;
          //String Str = "1231";
          Serial.println(Str);
          int a = 0;
          while (1) {
            //if(CompareTag(dataset[a][0],Str) == false){
            if (CompareTag(dataset[a][0], Str) == true) {
              Serial.println(Str + " : " + dataset[a][1]);
              LINE.notify(dataset[a][1] + " มาเรียน");
              break;
            }
            a++;
            //}
          }
          Str = "";
        }
      }
    }
    else {
      Serial.println("Bad3");
      Serial.println(Serial.peek(), HEX);
    }
  }
}

void setup() {
  Serial.begin(9600);
  LINE.setToken(LINE_TOKEN);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  ConvertToHid();

}
