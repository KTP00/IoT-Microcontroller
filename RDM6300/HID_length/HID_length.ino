//https://arduino.stackexchange.com/questions/64949/rdm6300-continuous-rfid-tag-read
// RDM6300 packet details
#define RDM6300_PACKET_SIZE  14
#define RDM6300_PACKET_BEGIN 0x02
#define RDM6300_PACKET_END   0x03

// Globack for reading RFID data
char buff[RDM6300_PACKET_SIZE];
uint32_t tag_id;
uint8_t checksum;
uint32_t Str ;
int a = 0;//เก็บเลขศูนย์ไว้ต่อในเลข HIP
int buzzer = 13;

void setup(void) {
  Serial.begin(9600);
 // Serial.println("Setting up...");
pinMode(buzzer,OUTPUT);
  // RFID board setup
  Serial.begin(9600);

 // Serial.println("READY. Scanning tags...");
}

void loop(void) {
  while (Serial.available() > 0) {
    if (Serial.peek() == RDM6300_PACKET_BEGIN || Serial.read()) {
      Serial.readBytes(buff, RDM6300_PACKET_SIZE);
      if (buff[13] != RDM6300_PACKET_END) {
        Serial.println("Bad1");
      }
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
          //Serial.print(tag_id);
        String Str = String(tag_id);
          for(;Str.length()<10;) 
           // for(;Str.length()<10;)           
            Str = String(a)+Str;
            String Str2 = String(Str);
          Serial.println(Str2);
          digitalWrite(buzzer,HIGH);
          delay(300);
          digitalWrite(buzzer,LOW);
          delay(300);
        /* 2 Loop =  Str = String(a)+Str;
                     Str = String(a)+Str;
                     Str = String(a)+Str; */ 
      }
    }}
    else {
      Serial.println("Bad3");
      Serial.println(Serial.peek(), HEX);
    }
    //Serial.println("END");
    tag_id = "";
  }
}
