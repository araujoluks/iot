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

#define ONLINE 1

#ifdef ONLINE
#include <PubSubClient.h>
#else
#include <SerialPubSubClient.h>
#endif

#include <Servo.h>

/******/
int SERVO = 6;  // definição do pino 6 para o servo
Servo sMotor;  // declaração do servo como objeto
int pos;  // posição do servo
int botao = 2;  // definição do pino 2 para o botão
int led = 3;   // definição do pino 3 para o led
int estadoBotao = LOW;   // variável para colher o estado atual do botão
int estadoPrev = LOW;   // variável para armazenar o último estado do botão
int aberto = 0;   // variável para verificar se o portão está aberto ou fechado
/******/

// Update these with values suitable for your network.
byte mac[] = {0xF0, 0x4D, 0xA2, 0x05, 0x80, 0x07} ; // change by your arduino mac address

char server[] = "test.mosquitto.org";
unsigned int port = 1883;

//char userId[] = "luksde.araujo@gmail.com"; // use your DIoTY user id (=email address)
//char passwd[] = "70df3401";  // use your DIoTY password

char userId[] = ""; // use your DIoTY user id (=email address)
char passwd[] = "";  // use your DIoTY password

// Callback function header
//void callback(char* topic, byte* payload, unsigned int length);
void quandoVierMensagemMQTT(char* topic, byte* payload, int length);

EthernetClient ethClient;
PubSubClient client(server, port, quandoVierMensagemMQTT, ethClient);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  
  Serial.println("Initialising");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  };

  Serial.println("Connected using DHCP");
  
  delay(1000);

  initPins();
  
  connectToMqttServer();
}

void initPins() {
  sMotor.attach(SERVO);
  sMotor.write(15); // Inicia motor posição zero
  pinMode(botao, INPUT);
  pinMode(led, OUTPUT);
}

int connectToMqttServer() {
  //if (client.connect("arduinoClient", userId, passwd)) {
  if (client.connect("arduinoClient-123123333")) {
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

void quandoVierMensagemMQTT(char* topic, byte* payload, int length) {
  int payloadComoInt = payload[0] - '0';
  Serial.println(String(topic));
  if (String(topic) == "/luksde.araujo@gmail.com/comandoPortao") {
    settarPortao(payloadComoInt);
  }
}

void togglePortao () {
  settarPortao(!aberto);
}

void settarPortao(int estado) {
  if (estado) {
    sMotor.write(115);
    delay(1000);
    digitalWrite(led, HIGH);
    aberto = 1;
  } else {
    sMotor.write(15);
    delay(1000);
    digitalWrite(led, LOW);
    aberto = 0;
  }

  //enviar mensagem de update
  //client.publish("portao", "1");
  client.publish("/luksde.araujo@gmail.com/portaoStatus", aberto);
}

void abrirPortao() {
  settarPortao(0);
  for (pos = 0; pos < 90; pos++) {
    sMotor.write(pos);
  }
  delay(1000);
  digitalWrite(led, HIGH);
  aberto = 1;
}

void fecharPortao() {
  settarPortao(1);
  for (pos = 90; pos > 0; pos--) {
    sMotor.write(pos);
    delay(20);
  }
  delay(1000);
  digitalWrite(led, LOW);
  aberto = 0;
}

void verificarBotaoPortao() {
  estadoBotao = digitalRead(botao);
  //Serial.println(estadoBotao);
  if (estadoBotao && !estadoPrev) {
    togglePortao();
  }
  estadoPrev = estadoBotao;
}

void loop() {
  client.loop();
  verificarBotaoPortao();
}
