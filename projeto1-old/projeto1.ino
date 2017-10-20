#include <SPI.h>
#include <PubSubClient.h>
#include <UIPEthernet.h>

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#endif

byte mac[] = {0xE0, 0xA2, 0xDA, 0x07, 0x88, 0x0E} ; // change by your arduino mac address
char userId[] = "luksde.araujo@gmail.com"; // use your DIoTY user id (=email address)
char passwd[] = "70df3401";  // use your DIoTY password
char server[] = "mqtt.dioty.co";
unsigned int port = 1883;
char topic[] = "/luksde.araujo@gmail.com/comandoPortao"; // topic where to publish; must be under your root topic
EthernetClient client;

void callback(char* topic, byte* payload, unsigned int length);

PubSubClient arduinoClient(server, port, callback, client);


void setup() {
  Serial.begin(9600);
  DEBUG_PRINT(F("Initialisation"));
  
  beginConnection();
}

void beginConnection() {
  DEBUG_PRINT(F("Entering beginConnection"));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    exit(-1);
  };
  DEBUG_PRINT(F("Obtained IP Address:"));
  DEBUG_PRINT(Ethernet.localIP());
  if (arduinoClient.connect(NULL, userId, passwd)) {
    DEBUG_PRINT(F("Connected to MQTT Server..."));
  } else {
    Serial.println("Failed to connect to the MQTT Server");
    Serial.flush();
    reConnect();
  }

  arduinoClient.subscribe(topic);
}

// reconnect after network hiccup
void reConnect() {
  DEBUG_PRINT(F("Entering reConnect"));

  if (arduinoClient.connected()) {
    DEBUG_PRINT(F("arduinoClient is connected"));
  } else {
    DEBUG_PRINT(F("arduinoClient is not connected"));
    //if (Ethernet.begin(mac) == 0) {
      //Serial.println(F("Failed to configure Ethernet using DHCP"));
    //};
    DEBUG_PRINT(Ethernet.localIP());
    if (arduinoClient.connect(NULL, userId, passwd)) {
      DEBUG_PRINT(F("Reconnected to MQTT Server..."));
    } else {
      Serial.println(F("Failed to connect to the MQTT Server"));
    }
  }

  arduinoClient.subscribe(topic);
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  /*
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
  */
}


void loop(void) {
  bool loop = arduinoClient.loop();

  if (loop) {
    DEBUG_PRINT(F("Arduino Client loop ok"));

    /*
    if ( !ds.search(addr)) {
      DEBUG_PRINT(F("No more addresses."));
      ds.reset_search();
      return;
    };
    if ( OneWire::crc8( addr, 7) != addr[7]) {
      DEBUG_PRINT(F("CRC is not valid!"));
      return;
    };
    */
    //getTemp();
    //dtostrf(((float)(Whole*100+Fract)/100),1,2, buf);

    //arduinoClient.publish(topic, i, 1, 1);
    
    


    delay(500);


  } else {
    DEBUG_PRINT(F("Arduino Client loop nok"));
    reConnect();
  }
  delay(5000);
}


