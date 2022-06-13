
//BIBLIOTECAS RFID:
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//CONFIGURAÇÃO DO BUZZER:
#define BUZZER 27

//DEFINIÇÕES RFID:
#define SS_PIN    5
#define RST_PIN   22 
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define pinVerde     12
#define pinVermelho  32
#define pinAmarelo  4
//esse objeto 'chave' é utilizado para autenticação
MFRC522::MIFARE_Key key;
//código de status de retorno da autenticação
MFRC522::StatusCode status; 
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
unsigned long UID_CARD = 0;

//CONFIGURAÇÃO DO DATALOGGER REMOTO: FIREBASE
const char * SHOWERUID = "projetomestradouft";
const char * lastUserCad;

int stateShower = 0;
bool showerStatusConnect = false;
bool showerOn = false;

//CONFIGURAÇÃO WIFI:
WiFiClient cliente;
const char * HOSTNAME = "showerCheap";
#define LED_CONNECT 13

//HOTSPOT:
const char * networkName = "Casa 2.4G";
const char * networkPswd = "13121993";

void setup() {
  
  Serial.begin(115200);
  delay(3000);
  
  pinMode(BUZZER, OUTPUT);
  
  //CONEXÃO COM A WIFI:
  connectToWiFi(networkName, networkPswd);
  pinMode(LED_CONNECT, OUTPUT);
  
  //DEFINIÇÕES RFID:
  SPI.begin(); // Init SPI bus
  pinMode(pinVerde, OUTPUT);
  pinMode(pinVermelho, OUTPUT);
  pinMode(pinAmarelo, OUTPUT);

  showerWaterSwitch(0);
  digitalWrite(pinVerde, LOW);
  digitalWrite(pinVermelho, HIGH); 
  
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o cartao do leitor...");
  Serial.println();
  
}

void loop() 
{
  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  testaConexao();
   
  Serial.println("Lendo cartão RFid...");
  
  UID_CARD = leituraDados();

  if(UID_CARD != 0)
  {
        cloudGetShowerData();
        userVerify();
  }
  
  // instrui o PICC quando no estado ACTIVE a ir para um estado de "parada"
  mfrc522.PICC_HaltA(); 
  // "stop" a encriptação do PCD, deve ser chamado após a comunicação com autenticação, caso contrário novas comunicações não poderão ser iniciadas
  mfrc522.PCD_StopCrypto1();  
    
  delay(500);
    

} // End of loop

unsigned long leituraDados()
{
  //imprime os detalhes tecnicos do cartão/tag
  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 

  //pega o UID do cartão
  unsigned long card_uid;
  card_uid =  mfrc522.uid.uidByte[0] << 24;
  card_uid += mfrc522.uid.uidByte[1] << 16;
  card_uid += mfrc522.uid.uidByte[2] <<  8;
  card_uid += mfrc522.uid.uidByte[3];
  
  //Prepara a chave - todas as chaves estão configuradas para FFFFFFFFFFFFh (Padrão de fábrica).
  for (byte i = 0; i < 6; i++) 
    key.keyByte[i] = 0xFF;
 
  //buffer para colocar os dados lidos
  byte buffer[SIZE_BUFFER] = {0};

  //variavel char que recebe os dados do buffer
  char id[MAX_SIZE_BLOCK] = {0};
  
  //bloco que faremos a operação
  byte bloco = 1;
  byte tamanho = SIZE_BUFFER;

  //variável de negacao de acesso
  int deny = 0;
 
  //faz a autenticação do bloco que vamos operar
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    return 0;
  }
 
  //faz a leitura dos dados do bloco
  status = mfrc522.MIFARE_Read(bloco, buffer, &tamanho);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    return 0;
  }
  else{
      digitalWrite(pinAmarelo, HIGH);
      delay(1000);
      digitalWrite(pinAmarelo, LOW);
  }
 
     
     //Serial.println(card_uid);
     return card_uid;

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

void cloudGetShowerData()
{
    
    //unsigned long startTime = millis();
    HTTPClient http;
 
    String addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/showers/" + String(SHOWERUID);
    
    http.begin(addr);
    
    Serial.println("cloudGetShowerData()");
   
    int httpCode = http.GET();

    String payload = http.getString();  // Gets the JSON from the provided URL
    //Serial.println(httpCode);  // Prints HTTP response code.

    // Prints the raw JSON. This line should probably be removed.
    //Serial.println(payload);
    

    // Calculates the size of the JSON buffer. Will probably explode
    // in the future. I don't really know what this line does.
    //const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 220;

    // Makes the JSON document with the buffer size calculated
    // earlier.
    DynamicJsonDocument doc(2048);

    // Deserializes the JSON we grabbed earlier.
    DeserializationError err = deserializeJson(doc, payload);

    // Prints out the deserialization error if an error occurred
    if (err) {
        Serial.print("JSON DESERIALIZE ERROR SH: ");
        Serial.println(err.c_str());
    }
    
    lastUserCad = doc["lastUserCad"];
    
    Serial.println(lastUserCad);
    
    http.end();
  
}

void userVerify()
{
  
  //SE USUÁRIO ESTIVER EM MODO DE CADASTRO PISCAR LUZES DO PAINEL POR ALGUNS SEGUNDOS E DEPOIS CANCELA

  //VER COMO FAZER FUNCIONAR O STATUS ONLINE DO SHOWER...
  
  if (WiFi.status() == WL_CONNECTED)
  {   
        HTTPClient http;
        String addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(lastUserCad);
        
        http.begin(addr);
        Serial.println("UserVerify");
        Serial.println(addr);
       
        int httpCode = http.GET();
    
        String payload = http.getString();  // Gets the JSON from the provided URL
        //Serial.println(httpCode);  // Prints HTTP response code.
    
        // Prints the raw JSON. This line should probably be removed.
        Serial.println(payload);
        
    
        // Calculates the size of the JSON buffer. Will probably explode
        // in the future. I don't really know what this line does.
        //const size_t capacity = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 220;
    
        // Makes the JSON document with the buffer size calculated
        // earlier.
        DynamicJsonDocument doc(4096);
    
        // Deserializes the JSON we grabbed earlier.
        DeserializationError err = deserializeJson(doc, payload);
    
        // Prints out the deserialization error if an error occurred
        if (err) {
            Serial.print("JSON DESERIALIZE ERROR US: ");
            Serial.println(err.c_str());
        }
        
        String cadInProgress = doc["cadInProgress"];
        String CLOUD_UID_CARD = doc["tagUID"];

        Serial.println(cadInProgress);

        if(cadInProgress == "true")
            cloudUserTagRegister();
        else
        {
            if(String(UID_CARD) == CLOUD_UID_CARD)
            {
                if(showerOn != true)
                {
                    showerWaterSwitch(1);
                    showerOn = true;
                    Serial.println("INICIAR CHUVEIRO!!");  
                    digitalWrite(pinVermelho, LOW);
                    digitalWrite(pinVerde, HIGH); 
                    delay(1000);
                    digitalWrite(pinVerde, LOW); 
                    delay(1000);
                    digitalWrite(pinVerde, HIGH);
                    delay(1000);
                    digitalWrite(pinVerde, LOW);
                    delay(1000);
                    digitalWrite(pinVerde, HIGH);
                    showerWaterSwitch(1);
                    showerOn = true;
                }
                else
                {
                    showerWaterSwitch(0);
                    showerOn = false;
                    Serial.println("FINALIZAR BANHO!!");  
                    digitalWrite(pinVerde, LOW);
                    digitalWrite(pinVermelho, HIGH); 
                    delay(1000);
                    digitalWrite(pinVermelho, LOW); 
                    delay(1000);
                    digitalWrite(pinVermelho, HIGH);
                    delay(1000);
                    digitalWrite(pinVermelho, LOW);
                    delay(1000);
                    digitalWrite(pinVermelho, HIGH);  
                    
                }
            }     
            else
            {
                showerWaterSwitch(0);
                showerOn = false;
                Serial.println("USUÁRIO DESCONHECIDO!!");
                digitalWrite(pinVerde, LOW);
                digitalWrite(pinVermelho, HIGH); 
                delay(1000);
                digitalWrite(pinVermelho, LOW); 
                delay(1000);
                digitalWrite(pinVermelho, HIGH);
                delay(1000);
                digitalWrite(pinVermelho, LOW);
                delay(1000);
                digitalWrite(pinVermelho, HIGH);      
            
            }   
        }
        http.end();
  }
  else
  {
     Serial.println("");
     Serial.println("Sem conexão com a Internet!");  
  }

}

void cloudUserTagRegister()
{
  cloudGetShowerData();
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    String addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(lastUserCad);
    Serial.println("TagRegister");
    Serial.println(addr);
    http.begin(addr);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    tempDocument["tagUID"] = UID_CARD;
    Serial.println("");
    Serial.println("Atualizando TagUID do Usuário: " + String(UID_CARD));
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

void showerWaterSwitch(int state)
{
    if (WiFi.status() == WL_CONNECTED)
    {   
      HTTPClient http;
      http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");
      
      http.addHeader("Content-Type", "application/json");
      StaticJsonDocument<50> tempDocument;
      char buffer[50];
      serializeJson(tempDocument, buffer);
      http.PATCH(buffer);
      http.end();
      //delay(500);
    }
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    String addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/showers/" + String(SHOWERUID);
    http.begin(addr);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    if(state)
        tempDocument["showerOn"] = true;
    else
        tempDocument["showerOn"] = false;
    
    char buffer[50];
    serializeJson(tempDocument, buffer);
    http.PATCH(buffer);
    http.end();
    //delay(500);
  }
  else
  {
     Serial.println("");
     Serial.println("Sem conexão com a Internet!");  
  }      
}

/*
void userVerify(String userUID)
{
  //VERIFICAR SE USER ESTÁ EM MODO DE USERCAD
  
  //cloudUserTagRegister(userUID); ?????

  //SEUSUÁRIO TIVER TAG REGISTRADA USA ELA SENÃO USA O LASTUSERCAD(CLOUDGETSHOWERDATA)

  //SE USUÁRIO ESTIVER EM MODO DE CADASTRO PISCAR LUZES DO PAINEL POR ALGUNS SEGUNDOS E DEPOIS CANCELA

  //VER COMO FAZER FUNCIONAR O STATUS ONLINE DO SHOWER...
  
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    String addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + userUID;
    Serial.println(addr);
    http.begin(addr);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    //stateShower = (stateShower + 1) % 2; // Flip stateShower
    //digitalWrite(pinVerde, stateShower);
    //digitalWrite(pinVermelho, !stateShower);
    //if(stateShower == 0) 
    //  showerOn = false;
    //else
    //  showerOn = true;
    
    tempDocument["tagUID"] = userUID;
    tempDocument["cadInProgress"]
    Serial.println("");
    Serial.println("Atualizando TagUID do Usuário: " + String(userUID));
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

}*/
