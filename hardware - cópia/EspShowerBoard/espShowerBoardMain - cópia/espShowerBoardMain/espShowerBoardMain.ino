//DOIT ESP32 DEV KIT
//CANAL I2C: PORTAS D21 E D22
//ENDEREÇOS I2C: ELÉTRICO:1 / HIDRÁULICO:2
//ENDEREÇO MAC: cc:50:e3:9a:82:14

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <time.h>
#include <sys/time.h>

//CONFIGURAÇÃO NTP:
IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
unsigned int LocalPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp;
unsigned int fuso = -3;//Fuso horário de Araguaina-TO
int ntpOK = 0;

//CONFIGURAÇÃO DO RTC:
struct tm data;//Cria a estrutura que contem as informacoes da data.
#define LINUX_TIME 1566831660 //LINUX TIME - https://www.unixtimestamp.com
unsigned long rtc_unix_time;
char data_LCD[64];
char hora_LCD[64];
char wday_LCD[64];

//CONFIGURAÇÃO DO BUZZER:
#define BUZZER 26

//CONFIGURAÇÃO VALVULA SOLENÓIDE:
#define SOLENOIDE 23

char* pEnd;
float powerCons = 0;
float waterCons = 0;

//CONFIGURAÇÃO DO DATALOGGER REMOTO: FIREBASE
const char * SHOWERUID = "projetomestradouft";
const char * userConnected;
String addr = "";

int stateShower = 0;
bool showerStatusConnect = false;
bool flagTrue = false;
#define SHOWER_STOPED 15
#define SHOWER_STARTED 16
String DATA_CSV;

//CONFIGURAÇÃO WIFI:
WiFiClient cliente;
const char * HOSTNAME = "showerCheap";
//#define LED_B 13
//HOTSPOT:
const char * networkName = "Casa 2.4G";
const char * networkPswd = "13121993";

//CONFIGURAÇÃO - RGB LED 
//OS LEDS BLUE ESTÃO CONECTADOS AO LED_B 13
#define LED_R 18
#define LED_G 19
#define LED_B 12


void setup() {
  pinMode(35, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(SHOWER_STOPED, OUTPUT);
  digitalWrite(SHOWER_STOPED, LOW);
  pinMode(SHOWER_STARTED, OUTPUT);
  digitalWrite(SHOWER_STARTED, LOW);
  
  digitalWrite(LED_R, HIGH);
  
  Wire.begin();
  pinMode(BUZZER, OUTPUT);
  pinMode(SOLENOIDE, OUTPUT);
  Serial.begin(115200);
  //CONEXÃO COM A WIFI:
  connectToWiFi(networkName, networkPswd);
  pinMode(LED_B, OUTPUT);
  //INICIALIZA
  initShower();
}

void loop() {

  testaConexao();
  ntp();
  rtcBuiltIn();
  delay(3000);
  showerGetData(); 

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
  float tempo, powerPot, powerKWH;
  char t[31]={};
  t[0] = '-';
  //CANAL 1:
  Serial.println("Solicitando fechamento de conta ao aferidor elétrico...");
  delay(2000);
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
      Serial.print("Dado bruto: ");
      Serial.print(t);
      Serial.println();
      
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
    
      Serial.print("Consumo de Energia em Moeda: ");
      powerCons = strtof(dados[2], &pEnd);
      Serial.print("R$");
      Serial.print(powerCons);

      Serial.println();

      powerKWH = (powerPot * (tempo*60))/3600000;
      
      Serial.print("Consumo de Energia em KWH: ");
      Serial.print(powerKWH);
      Serial.print(" KWh");
      Serial.println();
      
      Serial.println("--------------------------------------");

      Serial.println();

      powerConsuptionCloudUpdate(tempo, powerPot, powerCons, powerKWH);
      
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
  char t[31]={};
  float waterLit = 0;
  t[0] = '-';
  //CANAL 2:
  Serial.println("Solicitando fechamento de conta ao aferidor hidráulico...");
  delay(2000);
  Wire.requestFrom(2, 22);
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
      Serial.print("Dado bruto: ");
      Serial.print(t);
      Serial.println();
      
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

      Serial.print("Consumo de Água em Moeda: ");
      waterCons = strtof(dados[1], &pEnd);
      Serial.print("R$");
      Serial.print(waterCons);
      
      Serial.println();
      
      Serial.println("--------------------------------------");

      waterConsuptionCloudUpdate(waterLit, waterCons);
      
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
  digitalWrite(LED_B, LOW);
  Serial.println("");
  Serial.println("Buscando rede WiFi: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Pisca leds enquanto busca/conecta
    digitalWrite(LED_B, ledState);
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
  digitalWrite(LED_B, HIGH);
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
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/showers/" + String(SHOWERUID);
    http.begin(addr);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    tempDocument["showerStatusConnect"] = true;

    showerStatusConnect = true;
    
    Serial.println("");
    //Serial.println("Atualizando Status OnLine do Chuveiro: " + String(showerStatusConnect));
    Serial.println("Chuveiro pronto, aguardando comandos...");
    char buffer[50];
    serializeJson(tempDocument, buffer);
    http.PATCH(buffer);
    http.end();
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/showers/" + String(SHOWERUID);
    http.begin(addr);
    
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
    Serial.println("Comando remoto recebido: Ligar");  
    //delay(5000);      
}

void waterFluxOff()
{
    digitalWrite(SOLENOIDE, LOW);
    Serial.println("Comando remoto recebido: Desligar");
    dateTimeCloudUpdate();
    //delay(5000);
}

void showerGetData()
{
    
    if (WiFi.status() == WL_CONNECTED)
    {  
        unsigned long startTime = millis();
        HTTPClient http;
        addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/showers/" + String(SHOWERUID);

        //Serial.println(addr);
        
        http.begin(addr);
    
        int httpCode = http.GET();
    
        String payload = http.getString();  // Gets the JSON from the provided URL
        //Serial.println(httpCode);  // Prints HTTP response code.
    
        // Prints the raw JSON. This line should probably be removed.
        //Serial.println(payload);
        
    
        // Calculates the size of the JSON buffer. Will probably explode
        // in the future. I don't really know what this line does.
        const size_t capacity = 2 * JSON_OBJECT_SIZE(9) + JSON_ARRAY_SIZE(1) + 220;
    
        // Makes the JSON document with the buffer size calculated
        // earlier.
        DynamicJsonDocument doc(capacity);
    
        // Deserializes the JSON we grabbed earlier.
        DeserializationError err = deserializeJson(doc, payload);
    
        // Prints out the deserialization error if an error occurred
        if (err) {
            Serial.print("JSON DESERIALIZE SHOWER ERROR: ");
            Serial.println(err.c_str());
        }
    
       
        bool showerOn = doc["showerOn"];
        userConnected = doc["userConnected"];
        
        //Serial.println(showerOn);
        
        http.end();

        //if ((showerOn == true) || (digitalRead(35) == HIGH))        
        if(showerOn == true)
        {
            waterFluxOn();
            digitalWrite(SHOWER_STOPED, LOW);
            digitalWrite(SHOWER_STARTED, HIGH);
            digitalWrite(LED_B, LOW);
            digitalWrite(LED_R, LOW);
            digitalWrite(LED_G, HIGH);
            flagTrue = true;
        } 
        else 
        {
            if(flagTrue)
            {
                waterFluxOff();
                digitalWrite(LED_B, LOW);
                digitalWrite(LED_G, LOW);
                digitalWrite(LED_R, HIGH);
                
                //REQUISITAR DADOS DOS AFERIDORES:
                digitalWrite(SHOWER_STOPED, HIGH);
                digitalWrite(SHOWER_STARTED, LOW);
                Serial.println("");
                Serial.println("Enviando requisição...");
                Serial.print(".");
                delay(100);
                Serial.print(".");
                delay(100);
                Serial.print(".");
                delay(100);
                Serial.println(""); 
                dataSend(); 
                flagTrue = false;
            }
       
            
        }
        
    }
    else
    {
       Serial.println("");
       Serial.println("Sem conexão com a Internet!");  
    }
    
}

void powerConsuptionCloudUpdate(float tempo, float powerPot, float powerCons, float powerKWH)
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(userConnected);
    http.begin(addr);
    
    //Serial.println(userConnected);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<100> tempDocument;
    
    tempDocument["averBathTime"] = String(tempo);
    tempDocument["averPowerCons"] = String(powerCons);
    tempDocument["averPowerConsKWH"] = String(powerKWH);
    
    char buffer[100];
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

void waterConsuptionCloudUpdate(float waterLit, float waterCons)
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(userConnected);
    http.begin(addr);
    
    //Serial.println(userConnected);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    
    tempDocument["averWaterCons"] = String(waterCons);
    tempDocument["averWater"] = String(waterLit);
    
    char buffer[50];
    serializeJson(tempDocument, buffer);
    http.PATCH(buffer);
    http.end();
    delay(500);

    totalConsuptionCloudUpdate();
    
  }
  else
  {
     Serial.println("");
     Serial.println("Sem conexão com a Internet!");  
  } 
}

void totalConsuptionCloudUpdate()
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(userConnected);
    http.begin(addr);
    
    //Serial.println(userConnected);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;

    float consTot = powerCons + waterCons;
    
    tempDocument["consTot"] = String(consTot);
    
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

void ntp() 
{
  if(ntpOK == 0)
  {
      //get a random server from the pool
      WiFi.hostByName(ntpServerName, timeServerIP);
      sendNTPpacket(timeServerIP); // send an NTP packet to a time server
      // wait to see if a reply is available
      delay(6000);
    
      int cb = udp.parsePacket();
      if (!cb) {
        Serial.print("Pacote UDP/NTP não recebido");
      }
      else {
        Serial.print("Relógio atualizado com sucesso!");
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    
        // converte ntp em UNIX (epoch) usado como referencia
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        secsSince1900 += fuso * 60 * 60;// corrige a hora para o fuso desejado
        // now convert NTP time into everyday time:
        //Serial.print("Unix Time = ");//imprime os segundos desde 1970 para obter o valor época
        // Unix time starts on Jan 1 1970. Segundos desde 1970 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears;//  obtem valor em Unix time
        
        timeval tv;//Cria a estrutura temporaria para funcao abaixo.
        tv.tv_sec = epoch;//Atribui a data/hora UTC
        settimeofday(&tv, NULL);//Configura o RTC para manter a data atribuida atualizada.

        ntpOK = 1;
        
        //ACIONA BUZZER DUAS VEZES:
        digitalWrite(BUZZER, HIGH);    
        delay(200);
        digitalWrite(BUZZER, LOW);
        delay(200);
        digitalWrite(BUZZER, HIGH);    
        delay(200);
        digitalWrite(BUZZER, LOW);
        
        /*
        // imprime hora local
        char hora[30];// concatena a impressão da hora
        int  h, m, s;// hora, minuto, segundos
        h = (epoch  % 86400L) / 3600; // converte unix em hora (86400 equals secs per day)
        m = (epoch % 3600) / 60; // converte unix em minuto (3600 equals secs per minute)
        s = (epoch % 60); // converte unix em segundo
        sprintf( hora, "%02d:%02d:%02d", h, m, s);// concatena os valores h,m,s
    
        Serial.print("hora local:");
        Serial.println (hora);
        */
      }
      // wait ten seconds before asking for the time again
      //delay(10000);
  }
}

unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println();
  Serial.println("Atualizando o relógio via NTP...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void udpInit()
{
  Serial.println();
  Serial.print("Inicializando protocolo UDP na porta: ");
  Serial.println(LocalPort);
  udp.begin(LocalPort);
  //Serial.print("Local port: ");
  //Serial.println(udp.localPort());  
  Serial.println();
}

void rtcBuiltIn()
{
    
      vTaskDelay(pdMS_TO_TICKS(1000));//Espera 1 seg
   
   
      time_t tt = time(NULL);//Obtem o tempo atual em segundos. Utilize isso sempre que precisar obter o tempo atual
      data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura
   
      
      char data_formatada[64];
      strftime(data_formatada, 64, "%d/%m/%Y - %H:%M:%S", &data);//Cria uma String formatada da estrutura "data/hora" para serial
      strftime(hora_LCD, 64, "%H:%M", &data);//Cria uma String formatada da estrutura "hora" para LCD
      strftime(data_LCD, 64, "%d/%m/%Y", &data);//Cria uma String formatada da estrutura "data" para LCD
      strftime(wday_LCD, 64, "%a", &data);//Cria uma String formatada da estrutura "data" para LCD
      
      //printf("\nUnix Time: %d\n", int32_t(tt));//Mostra na Serial o Unix time
      printf("\nData atual: %s\n", data_formatada);//Mostra na Serial a data formatada

      //TESTA SE A DATA É ATUAL, SE NÃO FOR RETORNA "DATA DESATUALIZADA":
      DATA_CSV = data_formatada;
      
      /*
        
      if (tt >= 1566829200 && tt < 1566829210)//Use sua data atual, em segundos, para testar o acionamento por datas e horarios
      {
        printf("Acionando carga durante 3 segundos...\n");
      }
      */
}

void dateTimeCloudUpdate()
{
  if (WiFi.status() == WL_CONNECTED)
  {   
    HTTPClient http;
    addr = "https://showercheap-9ae49.firebaseapp.com/api/v1/users/" + String(userConnected);
    http.begin(addr);
    
    //Serial.println(userConnected);
    
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<50> tempDocument;
    
    tempDocument["dateTimeUserCons"] = DATA_CSV;
    
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
