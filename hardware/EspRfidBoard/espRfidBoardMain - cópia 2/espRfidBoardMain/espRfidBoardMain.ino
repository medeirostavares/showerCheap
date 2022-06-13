
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
unsigned long UID_CARD;

//CONFIGURAÇÃO DO DATALOGGER REMOTO: FIREBASE
const char * SHOWERUID = "projetomestradouft";

int stateShower = 0;
bool showerStatusConnect = false;

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
  
  
  // instrui o PICC quando no estado ACTIVE a ir para um estado de "parada"
  mfrc522.PICC_HaltA(); 
  // "stop" a encriptação do PCD, deve ser chamado após a comunicação com autenticação, caso contrário novas comunicações não poderão ser iniciadas
  mfrc522.PCD_StopCrypto1();  


    // AKI SE FAZ O PREENCHIMENTO DA MENSAGEM A SER ENVIADA
    //-----------------------------------------------------
    
    //String newValue = "Time since boot: " + String(millis()/1000);
    String userUID = String(UID_CARD);
    Serial.println("Enviando dados RFID via WIFI: \"" + userUID + "\"");
    Serial.println("");

    showerPanelUpdate(userUID);
    
    delay(4000);
    
//    // Read the value of the characteristic 2.
//    if(pRemoteCharacteristic2->canRead()) 
//    {
//      delay(3000);
//      std::string value2 = pRemoteCharacteristic2->readValue();
//      Serial.print("Status atual: ");
//      Serial.println(value2.c_str());
//      Serial.println("");
//      if(value2 == "LIBERADO")
//      {
//          digitalWrite(pinVermelho, LOW);
//          digitalWrite(pinVerde, HIGH);
//      }
//      else
//      {
//          if(value2 == "TRAVADO")
//          {            
//              digitalWrite(pinVermelho, HIGH);
//              digitalWrite(pinVerde, LOW);
//          }
//      }
//   }
    
//  }else if(doScan){
//    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
//  }
//  delay(1000); // Delay a second between loops.
//  
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

void showerPanelUpdate(String userUID)
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    stateShower = (stateShower + 1) % 2; // Flip stateShower
    digitalWrite(pinVerde, stateShower);
    digitalWrite(pinVermelho, !stateShower);
    if(stateShower == 0) 
      showerStatusConnect = false;
    else
      showerStatusConnect = true;
    tempDocument["showerOn"] = showerStatusConnect;
    Serial.println("");
    Serial.println("Atualizando Status do Chuveiro: " + String(showerStatusConnect));
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
