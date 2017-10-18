/*
 Example of using a Stream object to store the message payload

 Uses SRAM library: https://github.com/ennui2342/arduino-sram
 but could use any Stream based class such as SD

  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xAE, 0x0E, 0xB9 };
IPAddress ip(172, 16, 0, 100);
//IPAddress server(172, 16, 0, 2);
const char *mqtt_server = "m10.cloudmqtt.com";
const int mqtt_port = 15430;
const char *mqtt_user = "sfcnried";
const char *mqtt_pass = "6J4ac6Li1AgU";

bool LedState = false;

void callback(char* topic, byte* payload, unsigned int length) {
  //LedState = !LedState;
  //digitalWrite(0, LedState);  
  Serial.println(topic);
  //Serial.println(payload);
  Serial.println("FIM");
}

void ledCallback(char* topic, byte* payload, unsigned int length) {
  //LedState = !LedState;
  //digitalWrite(0, LedState);  
  Serial.println(topic);
  //Serial.println(payload);
  Serial.println("FIM LED CALLBACK");
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, mqtt_port, callback, ethClient);

void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  } else {
    Serial.println("Connected using DHCP");
  }

  if (client.connect("arduinoClient")) {
    client.publish("outTopic","hello world");
    client.subscribe("inTopic");
  }

  
  pinMode(0, OUTPUT);
}

void loop() {
  //Serial.println(client.connected());
  if (!client.connected()) {
    Serial.println("Connecting to MQTT server");
    if (client.connect("arduinoClient", mqtt_user, mqtt_pass)) {
      Serial.println("Connected to MQTT server");
      client.setCallback(ledCallback);  
      client.subscribe("test/buttonPressed");
    } else {
      Serial.println("Could not connect to MQTT server");   
    }
  }

  if (client.connected()) {
    client.loop();
  }
}
