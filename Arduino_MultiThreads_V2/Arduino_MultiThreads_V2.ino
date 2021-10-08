//Programacion Arduino Retina V1
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

int puertaIn = 51, puertaOut = 50, cambios = 0;

float Bus_Voltage[6] = {0, 0, 0, 0, 0, 0};
float Bus_Current[6] = {0, 0, 0, 0, 0, 0};
float Bus_Energy[6] = {0, 0, 0, 0, 0, 0};

float Bus_Banco[4] = {0, 0, 0, 0};

int tPZEM = 0;
int nPZEM = 0;

int tBank = 0;
int nBank = 0;
int working = 0;
int Bateria = 0;
int first = 1;
int Modulos[] = {38, 39, 40, 41};

float Bus[1] = {};
//======================================================

//====================== Pointers ======================
byte ipPZEM[6][4] = {{192, 168, 1, 1}, {192, 168, 1, 2}, {192, 168, 1, 3}, {192, 168, 1, 4}, {192, 168, 1, 5}, {192, 168, 1, 6}};
//======================================================

//=================== Loop Principal ===================
void setup() {
  Serial.begin(9600);
  attachInterrupt( 2, Interrupciones, CHANGE);

  pinMode (puertaOut, OUTPUT);
  for(int i; i <4; i++){
    pinMode(Modulos[i], OUTPUT);
    digitalWrite(Modulos[i], HIGH);
  }

}

void loop() {
  Temperatura();
  Humedad();
  //Lectura de PZEMS cada 1s
  if((millis()-tPZEM)>1000){
    tPZEM = millis();
    Read_PZEM(nPZEM);
    nPZEM++;
    if(nPZEM == 6){
      Send_PZEM();
      nPZEM = 0;
    }
  }
  //Lectura de Bancos de Baterias cada 0.250s
  if((millis()-tBank)>250){
    if(working == 0){
      working = Change_Bank(nBank, first);
      tBank = millis();
      first = 0;
    }
    else{
      working = Read_Bank(Bateria);
      Change_Bank(nBank, first);
      tBank = millis();
      nBank++;
      Bateria++;
      
      if(Bateria == 4){
        Send_Bank(nBank);
        Bateria = 0;
      }
      if(nBank == 16){
        nBank = 0;
        first = 1;
      }
    }
  }
  Lectura_Puerta();
  //====== Importante =====
  Bus[0] = cambios;
  Serial.print("/");
  Serial.print(Bus[0]);
  Serial.print("/");
  Serial.print("\r");
  cambios = 0;
  //============
  Combustible(); 
}
//======================================================

//=================== Interrupciones ===================
void Interrupciones() {
  cambios++;
  //Serial.println("pase");
  //delay(10);
}
//======================================================

//===================== Temperatura ====================
void Temperatura() {
  Serial.print("@");
  Serial.print(dht1.readTemperature());
  Serial.print(",");
  Serial.print(dht2.readTemperature());
  Serial.print(",");
  Serial.print(dht3.readTemperature());
  Serial.print(",");
  Serial.print(dht4.readTemperature());
  Serial.print("@");
  Serial.print("\r");
}
//======================================================

//====================== Humerdad ======================
void Humedad() {
  Serial.print("#");
  Serial.print(dht1.readHumidity());
  Serial.print(",");
  Serial.print(dht2.readHumidity());
  Serial.print(",");
  Serial.print(dht3.readHumidity());
  Serial.print(",");
  Serial.print(dht4.readHumidity());
  Serial.print("#");
  Serial.print("\r");
}
//======================================================

//===================== Read PZEM ======================
void Read_PZEM(int i){
  byte tempIP[4];
  for(int n = 0; n < 4; n++){
    tempIP[n] = ipPZEM[i][n];
  }
  Bus_Voltage[i] = pzem.voltage(tempIP);
  Bus_Current[i] = pzem.current(tempIP);
  Bus_Energy[i] = pzem.energy(tempIP);
}
//======================================================

//===================== Send PZEM ======================
void Send_PZEM(){
  int var = 0;
  Serial.print("%");
  while(var < 6){
    Serial.print(Bus_Voltage[var]);
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.print("%");
  Serial.print("\r");
  
  var = 0;
  Serial.print("&");
  while(var < 6){
    Serial.print(Bus_Current[var]);
    var++;
  }
  Serial.print("&");
  Serial.print("\r");
  
  var = 0;
  Serial.print("*");
  while(var < 6){
    Serial.print(Bus_Energy[var]);
    var++;
  }
  Serial.print("*");
  Serial.print("\r");
}
//======================================================

//===================== Read Bank ======================
int Read_Bank(int bateria){
  float Vbat = 0;
  const int medicion = A1;
  Vbat = analogRead(medicion) * 0.004887;
  Vbat = Vbat * 6 ;
  Bus_Banco[bateria] = Vbat;

  return 0;
}
//======================================================

//==================== Change Bank =====================
int Change_Bank(int i, int first){
  int bat = 22+i;
  if(first == 1){
    for (int bat = 22; bat < 38; bat++) {
      pinMode(bat, OUTPUT);
      digitalWrite(bat, LOW);
    }
  }

  if(digitalRead(bat) == LOW){
    digitalWrite(bat, HIGH);
  }
  else if(digitalRead(bat) == HIGH){
    digitalWrite(bat, LOW);
  }
  return 1;
}
//======================================================

//===================== Send Bank ======================
int Send_Bank(int i){
  int var = 0;
  if(i == 4){
    Serial.print("?");
  }
  else if(i == 8){
    Serial.print("[");
  }
  else if(i == 12){
    Serial.print("!");
  }
  else{
    Serial.print("]");
  }
  
  while(var < 4){
    Serial.print(Bus_Banco[var]);
    var++;
  }

  if(i == 4){
    Serial.print("?");
    Serial.print("\r");
  }
  else if(i == 8){
    Serial.print("[");
    Serial.print("\r");
  }
  else if(i == 12){
    Serial.print("!");
    Serial.print("\r");
  }
  else{
    Serial.print("]");
    Serial.print("\r");
  }
}
//======================================================

//================== Lectura Puerta ====================
void Lectura_Puerta() {
  pinMode (puertaOut, OUTPUT);
  digitalWrite (puertaOut, HIGH);
  Bus[0] = digitalRead(puertaIn);
  Serial.print("+");
  Serial.print(Bus[0]);
  Serial.print("+");
  Serial.print("\r");
}
//======================================================

//==================== Combustible =====================
void Combustible() {
  int Promedio = 0;
  for (int i = 0; i < 10; i++) {
    Promedio = (Promedio + analogRead(A2))/(i + 1);
    delay(10);
  }
  int var1 = 0;
  Serial.print(")");
  while(var1 < 1){
    Serial.print(Promedio);
    var1++;
  }
  Serial.print(")");
  Serial.print("\r");
}
//======================================================

//=============== Codificacion de Signos ===============
// @ = Temperatura
// # = Humedad
// % = Voltaje
// & = Corriente
// * = Potencia
// + = Lectura Puerta
// ) = Combustible
// ? = Banco de Baterias 1
// [ = Banco de Baterias 2
// ! = Banco de Baterias 3
// ] = Banco de Baterias 4
// / = Cambios Puerta
