/*
CÁLCULO DE CONSUMO DE ÁGUA:

TARIFAS:
R2 - R$6.18(11-15MC)
R3 - R$7.91(16-20MC)

RELAÇÃO MCxL
1L  = 0.001MC
1MC = 1000L

Ex. Tarifa R2: A cada 1000L = 1MC --> R$6.18

EQUAÇÃO:
A(R$) = (V(L) x 0.001) x TARIFA(R$)
RODANDO NO CANAL 2
*/

#include <Wire.h>

char q[10];
char g[10];
char a[10];
char strSend[25];
int require = -1;

byte statusLed    = 12;

//LITROS:
float waterLit = 0;
//R$:
float waterCons = 0;

bool fim_banho = false;

bool concluir = false;

byte sensorInterrupt = 0;  

//Digital pin 2
byte sensorPin       = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long litres;
unsigned long oldTime;

// TARIFA:
#define R2 6.18
#define R3 7.91
float A = 0;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);
  //CANAL 2
  Wire.begin(2); 
  delay(2000);
  Serial.println("--- Aferidor Hidráulico ---");
  Serial.println("Aguardando comandos..."); 
  Serial.println(); 
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
}

/**
 * Main program loop
 */
void loop()
{
  
  if(digitalRead(5))
  {
    fim_banho = true; 
    digitalWrite(12, LOW); 
    oldTime = 0;
    //tempo_minutos = 0;
  }
  else
  {
    if(digitalRead(6))
    {
        fim_banho = false; 
        digitalWrite(12, HIGH); 
        afereAgua();
        concluir = false;  
    }
  }
  
  if(fim_banho && (concluir == false))
  {
    delay(100);
    fechaConta();  
  }
  
}

void afereAgua()
{
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.println();
    Serial.print("Fluxo de água: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Quantidade de água utilizada: ");        
    litres = totalMilliLitres/1000;
    Serial.print(litres);
    Serial.println(" Litros"); 

    Serial.println();
    
    // CALCULA VALOR GASTO EM REAIS MEDIANTE TARIFA DA CONCESSIONÁRIA:
    
    A = ((totalMilliLitres/1000) * 0.001) * R2;
    Serial.print("Valor do consumo: R$");
    Serial.println(String(A));
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }  
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void fechaConta()
{
  Serial.println();
  Serial.println("------------------------");
  Serial.print("Fluxo de água: ");
  Serial.print(flowRate);
  Serial.print("L/min");

  Serial.println();

  Serial.print("Quantidade de água gasta: ");
  Serial.print(litres);
  Serial.print("L");
  
  Serial.println();
  
  Serial.print("Consumo Total: R$");
  Serial.print(A);
  Serial.println();
  Serial.println();
  Serial.println("Banho Concluído!");
  concluir = true;
  Serial.println("------------------------");
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
  
  dtostrf(litres, 5, 2, g);
  dtostrf(A, 5, 2, a);
  dtostrf(flowRate, 5, 2, q);

  Serial.println(g);
  Serial.println(a);
  Serial.println(q);
  
  sprintf(strSend, "%s ; %s ; %s ; " ,g,a,q);
  Wire.write(strSend);    
  Serial.println(strSend);   
}
