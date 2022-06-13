#include <BLEDevice.h>
//#include <BLEUtils.h>
#include <BLEServer.h>

//SERVIDOR BLUETOOTH...

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Link do gerador de UID's:
// https://www.uuidgenerator.net/

//CONFIGURAÇÃO BLE:
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC2_UUID "138550d6-569e-474e-a28e-3fc78b1129ec"
#define LED_READY 2
static BLECharacteristic *pCharacteristic;
static BLECharacteristic *pCharacteristic2;

//CONFIGURAÇÃO DO BUZZER:
#define BUZZER 26

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

void setup() 
{
  Serial.begin(115200);
  
  pinMode(BUZZER, OUTPUT);
  
  delay(3000);
  
  //CONEXÃO COM A WIFI:
  //connectToWiFi(networkName, networkPswd);
  pinMode(LED_CONNECT, OUTPUT);

  //INICIALIZANDO BLE:
  Serial.println("Inicializando BLE...");
  pinMode(LED_READY, OUTPUT);
  bluetoothBLEInit(); 
}

void loop() 
{
  //testaConexao();
  delay(5000);
  //showerStatusUpdate();
  bleCall();
  
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

void showerStatusUpdate()
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    http.begin("https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft");
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    stateShower = (stateShower + 1) % 2; // Flip stateShower
    digitalWrite(LED_READY, stateShower);
    if(stateShower == 0) 
      showerStatusConnect = false;
    else
      showerStatusConnect = true;
    tempDocument["showerStatusConnect"] = showerStatusConnect;
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

void bleCall()
{
  delay(2000);
  
  // Read the value of the characteristic.  
  std::string value = pCharacteristic->getValue();
  Serial.print("RFID UID: ");
  Serial.println(value.c_str());
  if(value == "2536558651")
  {
      digitalWrite(LED_READY, HIGH);
      pCharacteristic2->setValue("LIBERADO");
      digitalWrite(BUZZER, HIGH);    
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
      digitalWrite(BUZZER, HIGH);    
      delay(200);
      digitalWrite(BUZZER, LOW);
  }
  else
  {
      digitalWrite(LED_READY, LOW);
      pCharacteristic2->setValue("TRAVADO");
      digitalWrite(BUZZER, HIGH);    
      delay(800);
      digitalWrite(BUZZER, LOW);
      
  }
  std::string value2 = pCharacteristic2->getValue();
  Serial.print("STATUS: ");
  Serial.println(value2.c_str());  
}

void bluetoothBLEInit()
{
  BLEDevice::init("Chuveiro Inteligente");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic2 = pService->createCharacteristic(
                                         CHARACTERISTIC2_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );                                       

  pCharacteristic->setValue("Aguardando ID");
  pCharacteristic2->setValue("TRAVADO");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("");
}
