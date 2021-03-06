//#include <ArduinoJson.h>

#include <SPI.h>
#include <UIPEthernet.h>

#include <PubSubClient.h>


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xEE, 0xBD, 0xBE, 0x0F, 0xFE, 0xEE };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "192.168.3.140";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

int analogPin = A0;
int valAnalog;
int tempC;

char jsonString[25];

void setup() {
  //analogReference(INTERNAL);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  Serial.println(Ethernet.localIP());
}

void loop() {
  valAnalog = analogRead(analogPin);
  tempC = ( 5.0 * valAnalog * 100.0) / 1024.0;
  //tempC = valAnalog / 9.31;

  sprintf(jsonString, "{\"temp\": %d, \"id\": %d}", tempC, 9);
  Serial.println(jsonString);
  
  // if you get a connection, report back via serial:
  if (client.connect(server, 8080)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("POST / HTTP/1.1");
    client.println("Host: 192.168.3.140");
    client.println("Content-Type: application/json");
    client.println("Content-Length: 22");
    client.println("Connection: close");
    client.println();
    client.println(jsonString);
    //client.flush();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print("Resposta: ");
    Serial.print(c);
    Serial.println();
    
  }

  // if the server's disconnected, stop the client:
  /*
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
  */

  delay(1000);
  Serial.println("Once again...");
}

