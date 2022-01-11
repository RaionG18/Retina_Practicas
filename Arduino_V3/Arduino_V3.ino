//Programacion Arduino Retina V3
//===================== Librerias ======================
#include <SPI.h>
#include <SoftwareSerial.h>
#include <PZEM004T.h>
#include <DHT.h>
//======================================================

//================= Variables Globales =================
#define DHT1 42
#define DHT2 43
#define DHT3 44
#define DHT4 45
#define DHTTYPE DHT22
DHT dht1(DHT1, DHTTYPE), dht2(DHT2, DHTTYPE), dht3(DHT3, DHTTYPE), dht4(DHT4, DHTTYPE);
PZEM004T pzem(&Serial1);

byte ipPZEM[6][4] = {{192, 168, 1, 1}, {192, 168, 1, 2}, {192, 168, 1, 3}, {192, 168, 1, 4}, {192, 168, 1, 5}, {192, 168, 1, 6}};

float Bus_Voltage[6] = {0, 0, 0, 0, 0, 0};
float Bus_Current[6] = {0, 0, 0, 0, 0, 0};
float Bus_Energy[6] = {0, 0, 0, 0, 0, 0};

int nPZEM = 0;

int puertaIn = 51, puertaOut = 50, cambios = 0;
float Bus[1] = {};

int nBank = 0, Bateria = 0, Puerto = 22, work = 0;
int Modulos[] = {38, 39, 40, 41};
float Bus_Banco[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

unsigned long tBank = 0, tPZEM = 0, tMesg = 0;
//======================================================

//=================== Loop Principal ===================
void setup() {
  Serial.begin(9600);
  attachInterrupt( 2, Interrupciones, CHANGE);

  pinMode (puertaOut, OUTPUT);
  for(int a; a <4; a++){
    pinMode(Modulos[a], OUTPUT);
    digitalWrite(Modulos[a], LOW);
  }
  
  for (int a = 22; a < 38; a++) {
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
  }
  
  for(int a = 38; a < 42; a++){
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
  }
  
}

void loop() {

  //Lectura de PZEMS cada 1s
  if((millis()-tPZEM)>1000){
    tPZEM = millis();
    //Read_PZEM(nPZEM);
    nPZEM++;
    if(nPZEM == 6){
      nPZEM = 0;
    }
  }
  
  //Lectura de Bancos de Baterias cada 0.250s
  if((millis()-tBank)>250){

    if(work == 0){
      digitalWrite(Modulos[nBank], HIGH);
      digitalWrite(Puerto, HIGH);
      work = 1;
      tBank = millis();
    }

    else{
      Read_Bank(Bateria, nBank, Puerto);
      
      Bateria++;
      Puerto++;
      tBank = millis();
      
      if(Bateria == 4){
        digitalWrite(Modulos[nBank], LOW);
        Bateria = 0;
        nBank++;   
        if(nBank == 4){
          nBank = 0;
          Puerto = 22;
        }
      }
      work = 0;
    }       
  }

  //Mandar mensaje a Raspberry
  if((millis()-tMesg)>9000){
    Temperatura();
    Humedad();
    Send_PZEM();
    Send_Bank();
    Lectura_Puerta();
    //====== Importante =====
    Bus[0] = cambios;
    Serial.print("@");
    Serial.print(String(Bus[0]));
    Serial.println("@");
    cambios = 0;
    //=======================
    Combustible(); 
    tMesg = millis();
  }

  if(millis()<tMesg){
    tMesg = millis();
  }

  if(millis()<tBank){
    tBank = millis();
  }

  if(millis()<tPZEM){
    tPZEM = millis();
  }
}
//======================================================

//====================== Funciones =====================

//=================== Interrupciones ===================
void Interrupciones() {
  cambios++;
  //Serial.println("pase");
  //delay(10);
}
//======================================================

//===================== Read Bank ======================
void Read_Bank(int bateria, int banco, int bat){
  
  float Vbat = 0;
  const int medicion = A1;
  Vbat = analogRead(medicion) * 0.004887;
  Vbat = Vbat * 6 ;
  Bus_Banco[banco][bateria] = Vbat;

  digitalWrite(bat, LOW); 

}
//======================================================

//===================== Read PZEM ======================
void Read_PZEM(int i){
  byte tempIP[4];
  for(int a = 0; a < 4; a++){
    tempIP[a] = ipPZEM[i][a];
  }
  Bus_Voltage[i] = pzem.voltage(tempIP);
  Bus_Current[i] = pzem.current(tempIP);
  Bus_Energy[i] = pzem.energy(tempIP);
}
//======================================================

//==================== Send Banks ======================
void Send_Bank(){
  Serial.print("@");
  for (int a = 0; a < 4; a++){
    for (int b = 0; b < 4; b++){
      Serial.print(String(Bus_Banco[a][b]));
      Serial.print(",");
    }
    Serial.print(",");
  }
  Serial.println("@");
}
//======================================================

//===================== Send PZEM ======================
void Send_PZEM(){
  int var = 0;
  Serial.print("@");
  while(var < 6){
    Serial.print(String(Bus_Voltage[var]));
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.println("@");
  
  var = 0;
  Serial.print("@");
  while(var < 6){
    Serial.print(String(Bus_Current[var]));
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.println("@");
  
  var = 0;
  Serial.print("@");
  while(var < 6){
    Serial.print(String(Bus_Energy[var]));
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.println("@");
}
//======================================================

//===================== Temperatura ====================
void Temperatura() {
  Serial.print("@");
  Serial.print(String(dht1.readTemperature()));
  Serial.print(",");
  Serial.print(String(dht2.readTemperature()));
  Serial.print(",");
  Serial.print(String(dht3.readTemperature()));
  Serial.print(",");
  Serial.print(String(dht4.readTemperature()));
  Serial.println("@");
}
//======================================================

//====================== Humedad ======================
void Humedad() {
  Serial.print("@");
  Serial.print(String(dht1.readHumidity()));
  Serial.print(",");
  Serial.print(String(dht2.readHumidity()));
  Serial.print(",");
  Serial.print(String(dht3.readHumidity()));
  Serial.print(",");
  Serial.print(String(dht4.readHumidity()));
  Serial.println("@");
}
//======================================================

//================== Lectura Puerta ====================
void Lectura_Puerta() {
  pinMode (puertaOut, OUTPUT);
  digitalWrite (puertaOut, HIGH);
  Bus[0] = digitalRead(puertaIn);
  Serial.print("@");
  Serial.print(String(Bus[0]));
  Serial.println("@");
}
//======================================================

//==================== Combustible =====================
void Combustible() {
  int Promedio = 0;
  for (int a = 0; a < 10; a++) {
    Promedio = (Promedio + analogRead(A2))/(a + 1);
    delay(10);
  }
  Serial.print("@");
  Serial.print(String(Promedio));
  Serial.println("@");
}
//======================================================
