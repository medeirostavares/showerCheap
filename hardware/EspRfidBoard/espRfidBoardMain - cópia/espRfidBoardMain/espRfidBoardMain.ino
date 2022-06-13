//BIBLIOTECAS BLUETOOTH:
#include "BLEDevice.h"
#include "BLEScan.h"

//BIBLIOTECAS RFID:
#include <MFRC522.h>
#include <SPI.h>

//DEFINIÇÕES BLUETOOTH:
// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static BLEUUID    char2UUID("138550d6-569e-474e-a28e-3fc78b1129ec");
static boolean doConnect = false;
static boolean devConnected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemoteCharacteristic2;
static BLEAdvertisedDevice* myDevice;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Servidor encontrado: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

//DEFINIÇÕES RFID:
#define SS_PIN    5
#define RST_PIN   22 
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define pinVerde     12
#define pinVermelho  32
//esse objeto 'chave' é utilizado para autenticação
MFRC522::MIFARE_Key key;
//código de status de retorno da autenticação
MFRC522::StatusCode status; 
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
unsigned long UID_CARD;

void setup() {
  
  Serial.begin(115200);
  delay(3000);
  //DEFINIÇÕES BLUETOOTH:
  Serial.println("Iniciando Bluetooth...");
  Serial.println(" ");
  BLEDevice::init("client");
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  //DEFINIÇÕES RFID:
  SPI.begin(); // Init SPI bus
  pinMode(pinVerde, OUTPUT);
  pinMode(pinVermelho, OUTPUT);
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o cartao do leitor...");
  Serial.println();
  
} // End of setup.



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
   
  
  UID_CARD = leituraDados();
  
  
  // instrui o PICC quando no estado ACTIVE a ir para um estado de "parada"
  mfrc522.PICC_HaltA(); 
  // "stop" a encriptação do PCD, deve ser chamado após a comunicação com autenticação, caso contrário novas comunicações não poderão ser iniciadas
  mfrc522.PCD_StopCrypto1();  


  //BLOCO BLUETOOTH: ---------------------------
  
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("Conexão com o servidor estabelecida.");
    } else {
      Serial.println("Falha ao conectar ao servidor...");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (devConnected) {
    
    // AKI SE FAZ O PREENCHIMENTO DA MENSAGEM A SER ENVIADA
    //-----------------------------------------------------
    
    //String newValue = "Time since boot: " + String(millis()/1000);
    String newValue = String(UID_CARD);
    Serial.println("Enviando dados RFID via BLE: \"" + newValue + "\"");
    Serial.println("");
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());

    delay(2000);
    
    // Read the value of the characteristic 2.
    if(pRemoteCharacteristic2->canRead()) {
      std::string value2 = pRemoteCharacteristic2->readValue();
      Serial.print("Status atual: ");
      Serial.println(value2.c_str());
      Serial.println("");
      if(value2 == "TRAVADO")
      {
          digitalWrite(pinVermelho, HIGH);
          digitalWrite(pinVerde, LOW);
      }
      else
      {
          digitalWrite(pinVermelho, LOW);
          digitalWrite(pinVerde, HIGH);
      }
    }
    
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }
  delay(1000); // Delay a second between loops.
  
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
      digitalWrite(pinVerde, HIGH);
      delay(1000);
      digitalWrite(pinVerde, LOW);
  }
 
     
     //Serial.println(card_uid);
     return card_uid;

}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    devConnected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Estabelecendo conexão a ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Cliente criado");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Conectado ao chuveiro");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Falha ao encontrar a UUID do serviço: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Serviço encontrado");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    pRemoteCharacteristic2 = pRemoteService->getCharacteristic(char2UUID);
    if (pRemoteCharacteristic == nullptr || pRemoteCharacteristic2 == nullptr) {
      Serial.print("Falha ao encontrar alguma característica...");
      //Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Características encontradas");

    // Read the value of the characteristic 1.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("ID anterior: ");
      Serial.println(value.c_str());
    }

    // Read the value of the characteristic 2.
    if(pRemoteCharacteristic2->canRead()) {
      std::string value2 = pRemoteCharacteristic2->readValue();
      Serial.print("Status anterior: ");
      Serial.println(value2.c_str());
    }
    
    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    if(pRemoteCharacteristic2->canNotify())
      pRemoteCharacteristic2->registerForNotify(notifyCallback);

    devConnected = true;
}
