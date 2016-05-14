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
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
    
  #if STATIC
    ether.staticSetup(myip, gwip);
  #else
    if (!ether.dhcpSetup())
      Serial.println("DHCP failed");
  #endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}


double Fahrenheit(double celsius){
  return 1.8 * celsius + 32;
}

void loop(){

  Serial.println("\n");
    
  int chk = DHT11.read(DHT11PIN);
    
  Serial.print("Read sensor: ");
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
   
  delay(2000);

  
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
