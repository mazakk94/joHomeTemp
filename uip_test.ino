// http://hypernephelist.com/2014/07/11/arduino-uno-azure-mobile-services.html
#include <UIPEthernet.h>
#include <dht11.h>
#include <MemoryFree.h>
#include <Time.h>

dht11 DHT11;
#define DHT11PIN 2
const byte mac[]  = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
const char post[] = "POST /tables/temp_humi HTTP/1.1";

const char *server = "mazakk.azure-mobile.net";
//const char *table_name = "temp_humi";
const char *ams_key = "tIcJAmiZbjJJgQpTWHpMlFrdZDTXOy55";
int counter = 0;
int globalcounter = 0;
int resetPin = 8;

IPAddress ip(192, 168, 1, 102);

EthernetClient client;
char buffer[64];

void send_request(byte t, byte h){
  globalcounter = globalcounter + 1;
/*  time_t t = now();

 hour(t);          // Returns the hour for the given
                    //  time t
  minute(t);        // Returns the minute for the given
                    //  time t
  second(t);        // Returns the second for the given
                    //  time t 
  day(t);           // The day for the given time t

  month(t);         // The month for the given time t

  year(t);          // The year for the given time t
 
*/

  
 //Serial.println(curr_time);
  Serial.println(F("connecting..."));
 /* byte len = strlen_P(serv);
  byte k;
  char myBuff[len];
  for (k = 0; k < len; k++)  {
    myBuff[k] = pgm_read_byte_near(serv + k);
    Serial.print(myBuff[k]); 
  }
  myBuff[strlen_P(myBuff)+1] = '\0';


Serial.println();
  Serial.println(myBuff);
  */
  if (client.connect(server, 80)) {

    //sprintf(buffer, "POST /tables/%s HTTP/1.1", table_name);
    sprintf(buffer,"%s", post);
    client.println(buffer);
    
    sprintf(buffer, "Host: %s", server);
    client.println(buffer);
    
    sprintf(buffer, "X-ZUMO-APPLICATION: %s", ams_key);
    client.println(buffer);
    client.println("Content-Type: application/json");
    sprintf(buffer, "{\"tmp\": %d, \"humi\": %d}", t, h);
    
    client.print("Content-Length: ");
    client.println(strlen(buffer));
    client.println();
    client.println(buffer);
    
  } else {
    Serial.println(F("conn fail"));
  }
}
 

void wait_response(){
  while (!client.available()) {
    if (!client.connected()) {
      return;
    }
  }
}

void read_response()
{
  bool print = true;
  bool count = false;
  while (client.available()) {
    char c = client.read();
    if (c == '\n'){
      print = false;
    }
    if (print){
      count = true;
      Serial.print(c);
    }
  }
  
  if (!count){
    counter = counter + 1;
  }
  Serial.println();
}

void end_request() {
  client.stop();
}


void(* resetFunc) (void) = 0;


void setup() {
  counter = 0;
  globalcounter = 0;
  Serial.begin(9600);
  while (!Serial) ;

  //digitalWrite(resetPin, HIGH);
 // delay(200);   
 // pinMode(resetPin, OUTPUT);     
  //Serial.println("reset");
  delay(200);
  
  Serial.print(F("lib:"));
  Serial.println(DHT11LIB_VERSION);
  Serial.println(F("initializing ethernet.."));
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Ethernet failed! Need to reset"));
    delay(1000); 
    Serial.println(F("resetting..."));
    delay(1000);
    resetFunc();
    delay(1000);
    Ethernet.begin(mac, ip);
    for (;;) ;
  }
  Serial.println(F("Ethernet connected successfully"));
  
  delay(1000);
}

void loop(){
  // int val = analogRead(A0);
  byte chk = DHT11.read(DHT11PIN);
  byte t = (int)DHT11.temperature;
  byte h = (int)DHT11.humidity;
  send_request(t, h);
  wait_response();
  read_response();
  end_request();
  
  Serial.print(F("Counter: "));
  Serial.print(counter);
  Serial.print(F(", Global counter: "));
  Serial.println(globalcounter);
  if(counter >= 5 || globalcounter >= 50){
    delay(1000); 
    Serial.println(F("resetting..."));
    delay(1000);
    resetFunc();
    delay(1000);
  }
  delay(5*1000);
}
