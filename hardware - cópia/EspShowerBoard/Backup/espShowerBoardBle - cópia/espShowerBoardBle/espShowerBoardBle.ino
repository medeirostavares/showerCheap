
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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
#define LED_READY 15
//HOTSPOT:
const char * networkName = "Wifi - IFTO";
const char * networkPswd = "";

void setup() 
{
  Serial.begin(115200);
  
  pinMode(BUZZER, OUTPUT);
  
  delay(3000);
  
  //CONEXÃO COM A WIFI:
  //connectToWiFi(networkName, networkPswd);
  pinMode(LED_CONNECT, OUTPUT);

}

void loop() 
{
  testaConexao();
  delay(5000);
  showerStatusUpdate();
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
