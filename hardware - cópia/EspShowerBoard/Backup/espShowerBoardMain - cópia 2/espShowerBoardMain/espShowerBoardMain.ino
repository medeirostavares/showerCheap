//DOIT ESP32 DEV KIT
//CANAL I2C: PORTAS D21 E D22
//ENDEREÇOS I2C: ELÉTRICO:1 / HIDRÁULICO:2
//ENDEREÇO MAC: 80:7D:3A:B7:DD:D4

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//CONFIGURAÇÃO DO BUZZER:
#define BUZZER 26

//CONFIGURAÇÃO VALVULA SOLENÓIDE:
#define SOLENOIDE 2

//char t[25]={};
//float tempo, power, water;
char* pEnd;

//CONFIGURAÇÃO DO DATALOGGER REMOTO: FIREBASE
const char * SHOWERUID = "projetomestradouft";

int stateShower = 0;
bool showerStatusConnect = false;

//CONFIGURAÇÃO WIFI:
WiFiClient cliente;
const char * HOSTNAME = "showerCheap";
#define LED_CONNECT 13
#define LED_READY 15
//HOTSPOT:
const char * networkName = "Casa 2.4G";
const char * networkPswd = "13121993";

//CONFIGURAÇÃO - RGB LED 
//OS LEDS BLUE ESTÃO CONECTADOS AO LED_CONNECT 13
#define LED_R 18
#define LED_G 19


void setup() {
  pinMode(35, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  digitalWrite(LED_R, HIGH);
  
  Wire.begin();
  pinMode(BUZZER, OUTPUT);
  pinMode(SOLENOIDE, OUTPUT);
  Serial.begin(115200);
  //CONEXÃO COM A WIFI:
  connectToWiFi(networkName, networkPswd);
  pinMode(LED_CONNECT, OUTPUT);
  //INICIALIZA
  initShower();
}

void loop() {

  testaConexao();
  delay(3000);
  showerGetData();
  //waterFluxOn();

// if(digitalRead(35))
// {
//    Serial.println("Enviando requisição...");
//    Serial.print(".");
//    delay(100);
//    Serial.print(".");
//    delay(100);
//    Serial.print(".");
//    delay(100);
//    Serial.println(""); 
//    dataSend(); 
// } 

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


void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;
  digitalWrite(LED_CONNECT, LOW);
  Serial.println("");
  Serial.println("Buscando rede WiFi: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Pisca leds enquanto busca/conecta
    digitalWrite(LED_CONNECT, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED)
  {
        digitalWrite(LED_R, LOW);
  }
  WiFi.setHostname(HOSTNAME);
  Serial.println();
  Serial.println("Conectado à rede WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Potência do sinal: ");
  Serial.println(WiFi.RSSI());
  Serial.print("Endereço MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Nome de Host: ");
  Serial.println(WiFi.getHostname());
  Serial.println();
  digitalWrite(LED_CONNECT, HIGH);
  //ACIONA BUZZER UMA VEZ:
  digitalWrite(BUZZER, HIGH);    
  delay(200);
  digitalWrite(BUZZER, LOW);

  //INICIALIZA PROTOCOLO UDP PARA UTC:
  //udpInit();
}

void testaConexao()
{
  
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    Serial.println();
    Serial.println("Wifi desconectado...");
    Serial.println();
    Serial.println("Reconectando...");
    Serial.println();
    //REEFETUA TENTATIVA DE CONEXÃO:
    connectToWiFi(networkName, networkPswd);
  }
  
}

void initShower()
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    tempDocument["showerStatusConnect"] = true;
    
    Serial.println("");
    Serial.println("Atualizando Status do Chuveiro: " + String(showerStatusConnect));
    char buffer[50];
    serializeJson(tempDocument, buffer);
    http.PATCH(buffer);
    http.end();
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    tempDocument["showerOn"] = false;
    
    char buffer[50];
    serializeJson(tempDocument, buffer);
    http.PATCH(buffer);
    http.end();
    delay(500);
  }
  else
  {
     Serial.println("");
     Serial.println("Sem conexão com a Internet!");  
  }
  
}

void waterFluxOn()
{
    digitalWrite(SOLENOIDE, HIGH);
    //delay(5000);      
}

void waterFluxOff()
{
    digitalWrite(SOLENOIDE, LOW);
    //delay(5000);
}

void showerGetData()
{
    
    unsigned long startTime = millis();
    HTTPClient http;

    http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");

    int httpCode = http.GET();

    String payload = http.getString();  // Gets the JSON from the provided URL
    //Serial.println(httpCode);  // Prints HTTP response code.

    // Prints the raw JSON. This line should probably be removed.
    //Serial.println(payload);
    

    // Calculates the size of the JSON buffer. Will probably explode
    // in the future. I don't really know what this line does.
    const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 220;

    // Makes the JSON document with the buffer size calculated
    // earlier.
    DynamicJsonDocument doc(capacity);

    // Deserializes the JSON we grabbed earlier.
    DeserializationError err = deserializeJson(doc, payload);

    // Prints out the deserialization error if an error occurred
    if (err) {
        Serial.print("JSON DESERIALIZE ERROR: ");
        Serial.println(err.c_str());
    }

   
    bool showerOn = doc["showerOn"];
    
    Serial.println(showerOn);
    
    http.end();
    
    if (showerOn == true) 
    {
        waterFluxOn();
        digitalWrite(LED_CONNECT, LOW);
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, HIGH);
    } 
    else 
    {
        waterFluxOff();
        digitalWrite(LED_CONNECT, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_R, HIGH);
    }
    
//    unsigned long endTime = (millis() - startTime);
//    Serial.print("LED GET TOOK: ");
//    Serial.print(endTime);
//    Serial.println("ms");
//    Serial.print("Running on core ");
//    Serial.println(xPortGetCoreID());
//    Serial.println("------------------------------");
    
}
