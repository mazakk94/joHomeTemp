// Present a "Will be back soon web page", as stand-in webserver.
// 2011-01-30 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php
 
#include <EtherCard.h>
#include <dht11.h>

dht11 DHT11;
#define DHT11PIN 2
#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
  static byte myip[] = { 192,168,1,180 }; // ethernet interface ip address
  static byte gwip[] = { 192,168,1,254 }; // gateway ip address
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer


//EthernetClient client;

char buff[64];


const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Hello Arduino!"
  "</title></head>"
  "<body>"
    "<h3>Test Arduino</h3>"
    "<p><em>"
      "Wszystko bangla :)"
    "</em></p>"
  "</body>"
"</html>"
;

void setup(){
  //Serial.begin(57600);
  Serial.begin(9600);
  Serial.println("\n[backSoon]");

  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();

/*
  int errorCheck = ether.begin(sizeof Ethernet::buffer, mymac);
  
  if (errorCheck == 0)
    Serial.println( "Failed to access Ethernet controller");
  else 
    Serial.println(errorCheck);
    
  #if STATIC
    ether.staticSetup(myip, gwip);
  #else
    if (!ether.dhcpSetup())
      Serial.println("DHCP failed");
  #endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
*/
}


double Fahrenheit(double celsius){
  return 1.8 * celsius + 32;
}

/*
void send_request(int value){
  Serial.println("\nconnecting...");

  if (client.connect(server, 80)) {

    Serial.print("sending ");
    Serial.println(value);

    // POST URI
    sprintf(buff, "POST /tables/%s HTTP/1.1", table_name);
    client.println(buff);

    // Host header
    sprintf(buff, "Host: %s", server);
    client.println(buff);

    // Azure Mobile Services application key
    sprintf(buff, "X-ZUMO-APPLICATION: %s", ams_key);
    client.println(buff);

    // JSON content type
    client.println("Content-Type: application/json");

    // POST body
    sprintf(buff, "{\"value\": %d}", value);

    // Content length
    client.print("Content-Length: ");
    client.println(strlen(buff));

    // End of headers
    client.println();

    // Request body
    client.println(buff);
    
  } else {
    Serial.println("connection failed");
  }
}
*/
void loop(){

  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case 0: Serial.println("OK"); break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
      
  Serial.print("Wilgotnosc (%): ");
  Serial.println((float)DHT11.humidity, 2);
    
  Serial.print("Temperatura (oC): ");
  Serial.println((float)DHT11.temperature, 2);
    
  Serial.print("Temperatura (oF): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);

  int num = (int)DHT11.temperature;
  Serial.println("b4 send");
  Serial.println(num);
  //send_request(num);


   
  delay(2000);

  
  // wait for an incoming TCP packet, but ignore its contents
  /*
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
  */
}
