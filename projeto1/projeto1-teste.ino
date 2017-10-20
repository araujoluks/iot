/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.

*/

#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[] = {0xE0, 0xA2, 0xDA, 0x07, 0x88, 0x0E} ; // change by your arduino mac address

char server[] = "mqtt.dioty.co";
unsigned int port = 1883;

char userId[] = "luksde.araujo@gmail.com"; // use your DIoTY user id (=email address)
char passwd[] = "70df3401";  // use your DIoTY password

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, port, callback, ethClient);

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.

  client.publish("/luksde.araujo@gmail.com/mensagemRecebida", "1", length);
  
  Serial.println(topic);
  //for (int i = 0; i < length; i++) {
      //Serial.print((char)payload[i]);
  //}

  //Serial.println();

  payload[length] = '\0';
  char* payloadChar = payload;
  String s = String(payloadChar);
  int f = s.toInt();


  Serial.println(s);
  Serial.println(f);
  
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  };

  Serial.println("Connected using DHCP");
  
  delay(1000);
  
  connectToMqttServer();
}

int connectToMqttServer() {
  if (client.connect("arduinoClient", userId, passwd)) {
    client.publish("/luksde.araujo@gmail.com/arduinoLigando","1");
    client.subscribe("/luksde.araujo@gmail.com/comandoPortao");
    Serial.println("Connected to MQTT Server");
    Serial.flush();
  } else {
    Serial.println("Failed to connect to MQTT Server");
    Serial.flush();
    connectToMqttServer();
  }
}

void loop()
{
  client.loop();
}
