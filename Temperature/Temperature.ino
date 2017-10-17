/*Temperatura em Celsius*/
 
int analogPin = A0;
int valAnalog;
float tempC;

void setup() {
  analogReference(INTERNAL);
  Serial.begin(9600);
}
 
void loop() {
  valAnalog = analogRead(analogPin);
  //tempC = ( 5.0 * valAnalog * 100.0) / 1024.0;
  tempC = valAnalog / 9.31;
  Serial.println(tempC) ;
  delay(1000) ;
}
