//DOIT ESP32 DEV KIT
//CANAL I2C: PORTAS D21 E D22
//ENDEREÇOS I2C: ELÉTRICO:1 / HIDRÁULICO:2


#include <Wire.h>

//char t[25]={};
//float tempo, power, water;
char* pEnd;

void setup() {
  pinMode(35, INPUT);
  Wire.begin();
  Serial.begin(115200);
  delay(2000);
  Serial.println("--- Shower Board ---");
  Serial.println();
}

void loop() {
 delay(100);
 if(digitalRead(35))
 {
    Serial.println("Enviando requisição...");
    Serial.print(".");
    delay(100);
    Serial.print(".");
    delay(100);
    Serial.print(".");
    delay(100);
    Serial.println(""); 
    dataSend(); 
 } 

}

void dataSend()
{
    
    //MENSAGEM DE TÉRMINO DE BANHO PARA O AFERIDOR ELÉTRICO:
    Wire.beginTransmission(1);
    Wire.write(0);
    Wire.endTransmission(); 
    requestDataPower();
    
    
    //MENSAGEM DE TÉRMINO DE BANHO PARA O AFERIDOR HIDRÁULICO:
    Wire.beginTransmission(2);
    Wire.write(0);
    Wire.endTransmission();     
    requestDataWater();
    
    delay(500);
   
}

void requestDataPower()
{
  float tempo, powerPot, powerCons;
  char t[25]={};
  t[0] = '-';
  //CANAL 1:
  Wire.requestFrom(1, 22);
  int i=0;
  while (Wire.available()) 
  { 
     delay(100);
     t[i] = Wire.read();
     i=i+1;
  }
  if(t[0] != '-')
  {
      Serial.println("DADOS DO AFERIDOR ELÉTRICO:");
      //Serial.print("Dado bruto: ");
      //Serial.print(t);
      //Serial.println();
      
      int count=0,j;
      for(j=0;t[j];j++)
      {
         if(t[j]!=' ')
         {
             t[count++]=t[j];
         }
      }
      t[count]=0;
      count=0;
      //Serial.print("Sem espaços: ");
      //Serial.print(t);
      //Serial.println();
      
      char *letra = t;
      int separadores = 0;
      int k = 0;
    
      while (*letra != '\0')
      {
          if (*(letra++) == ';') 
            separadores++;
      }
      
      char* dados[separadores]; //criar o array de palavras interpretadas
      char *palavra = strtok(t, ";"); //achar a primeira palavra com strtok
      
      while (palavra != NULL)
      { //se já chegou ao fim devolve NULL
          dados[k++] = palavra; //guardar a palavra corrente e avançar
          palavra = strtok(NULL, ";"); //achar a próxima palavra
      }
      
      Serial.print("Tempo do Banho: ");
      tempo = strtof(dados[0], &pEnd); 
      Serial.print(tempo);
      Serial.print(" Minutos");
    
      Serial.println();
    
      Serial.print("Potência Média: ");
      powerPot = strtof(dados[1], &pEnd);
      Serial.print(powerPot);
      Serial.print(" Watts");
    
      Serial.println();
    
      Serial.print("Consumo de Energia: ");
      powerCons = strtof(dados[2], &pEnd);
      Serial.print("R$");
      Serial.print(powerCons);
      Serial.println();
      Serial.println("--------------------------------------");
    }
    else
    {
      Serial.println();
      Serial.println("Falha na comunicação!");
      Serial.println("Verifique se o aferidor elétrico está online...");  
      Serial.println();
    }
}

void requestDataWater()
{
  char t[25]={};
  t[0] = '-';
  float waterLit, waterCons;
  //CANAL 2:
  Wire.requestFrom(2, 12);
  int i=0;
  while (Wire.available()) 
  { 
     delay(100);
     t[i] = Wire.read();
     i=i+1;
  }
  if(t[0] != '-')
  {
      Serial.println("DADOS DO AFERIDOR HIDRÁULICO:");
      //Serial.print("Dado bruto: ");
      //Serial.print(t);
      //Serial.println();
      
      int count=0,j;
      for(j=0;t[j];j++)
      {
         if(t[j]!=' ')
         {
             t[count++]=t[j];
         }
      }
      t[count]=0;
      count=0;
      //Serial.print("Sem espaços: ");
      //Serial.print(t);
      //Serial.println();
      
      char *letra = t;
      int separadores = 0;
      int k = 0;
    
      while (*letra != '\0')
      {
          if (*(letra++) == ';') 
            separadores++;
      }
      
      char* dados[separadores]; //criar o array de palavras interpretadas
      char *palavra = strtok(t, ";"); //achar a primeira palavra com strtok
      
      while (palavra != NULL)
      { //se já chegou ao fim devolve NULL
          dados[k++] = palavra; //guardar a palavra corrente e avançar
          palavra = strtok(NULL, ";"); //achar a próxima palavra
      }
      
      
      Serial.print("Quantidade de Água: ");
      waterLit = strtof(dados[0], &pEnd); 
      Serial.print(waterLit);
      Serial.print(" Litros");
    
      Serial.println();
    
      Serial.print("Consumo de Água: ");
      waterCons = strtof(dados[1], &pEnd);
      Serial.print("R$");
      Serial.print(waterCons);
      Serial.println();
  }
  else
  {
      Serial.println();
      Serial.println("Falha na comunicação!");
      Serial.println("Verifique se o aferidor hidráulico está online...");  
      Serial.println();
  }
}
