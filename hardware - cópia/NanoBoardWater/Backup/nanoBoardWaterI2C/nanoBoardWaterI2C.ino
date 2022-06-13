/*
//FLOAT EM STRING
char t[10]; //empty array
float x = 3.38;
dtostrf(x, 3, 2, t); //(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
Wire.write(t);  
*/

//PORTAS I2C: A4 E A5
// RODANDO NO CANAL 2

#include <Wire.h>

char q[10];
char g[10];
char strSend[12];
int require = -1;

byte statusLed    = 12;

//LITROS:
float waterLit = 0;
//R$:
float waterCons = 0;

void setup()
{
  Serial.begin(115200);
  //CANAL 2
  Wire.begin(2);
  delay(2000);
  Serial.println("--- Aferidor Hidráulico ---");
  Serial.println(); 
  digitalWrite(statusLed, HIGH);
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
}

void loop()
{
    
}

void receiveEvent(int howMany)
{
  if(Wire.available())
  {
    require = Wire.read();
  }
  Serial.println("Recebendo requisição...");
  Serial.println();
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
  Serial.println("");
}

void requestEvent()
{
  Serial.println("Enviando dados do banho...");
    
    dtostrf(waterLit, 6, 2, q);
    dtostrf(waterCons, 6, 2, g);
    sprintf(strSend, "%s ; %s",q,g);
    Wire.write(strSend);    
    Serial.println(strSend);   
}
