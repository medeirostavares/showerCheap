#include <SoftwareSerial.h>
 
// Pino TXD -> Digital ??
// Pino RXD -> Digital ??
SoftwareSerial mySerial(16,17);
 
void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Aguardando Comandos!");
  Serial.println("Favor setar NL e CR no monitor serial...");
  mySerial.begin(9600);
}
 
// Laço de Repetição
void loop() {
  if (mySerial.available())
  Serial.write(mySerial.read());
  if (Serial.available())
  mySerial.write(Serial.read());
}
