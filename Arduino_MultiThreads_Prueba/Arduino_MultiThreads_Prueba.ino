//===================== Librerias ======================
#include <SPI.h>
#include <SoftwareSerial.h>
#include <PZEM004T.h>
#include <DHT.h>
#include <pt.h>
//======================================================

//======================== Hilos =======================
struct pt Hilo1;
struct pt Hilo2;
//======================================================

//================= Variables Globales =================
#define DHT1 42
#define DHT2 43
#define DHT3 44
#define DHT4 45
#define DHTTYPE DHT22
//======================================================

//===================== Variables ======================
DHT dht1(DHT1, DHTTYPE), dht2(DHT2, DHTTYPE), dht3(DHT3, DHTTYPE), dht4(DHT4, DHTTYPE);
PZEM004T pzem(&Serial1);

int bateria1 = 22, bateria2 = 23, bateria3 = 24, bateria4 = 25;
int bateria5 = 26, bateria6 = 27, bateria7 = 28, bateria8 = 29;
int bateria9 = 30, bateria10 = 31, bateria11 = 32, bateria12 = 33;
int bateria13 = 34, bateria14 = 35, bateria15 = 36, bateria16 = 37;

int modulo1 = 38, modulo2 = 39, modulo3 = 40, modulo4 = 41;

int puertaIn = 51, puertaOut = 50;
int cambios = 0;
int ventilador = 46;
int inclinometro = 47;
int movimiento = 49;

byte ipPZEM1[] = {192, 168, 1, 1};
byte ipPZEM2[] = {192, 168, 1, 2};
byte ipPZEM3[] = {192, 168, 1, 3};
byte ipPZEM4[] = {192, 168, 1, 4};
byte ipPZEM5[] = {192, 168, 1, 5};
byte ipPZEM6[] = {192, 168, 1, 6};

byte pzems[] = { 
  &ipPZEM1, 
  &ipPZEM2, 
  &ipPZEM3, 
  &ipPZEM4, 
  &ipPZEM5, 
  &ipPZEM6
  };

float Bus[1] = {};
float Bus_Voltage[6] = {0, 0, 0, 0, 0, 0};
float Bus_Current[7] = {0, 0, 0, 0, 0, 0};
float Bus_Energy[7] = {0, 0, 0, 0, 0, 0};

float Bus_Banco1[4] = {0, 0, 0, 0};
float Bus_Banco2[4] = {0, 0, 0, 0};
float Bus_Banco3[4] = {0, 0, 0, 0};
float Bus_Banco4[4] = {0, 0, 0, 0};
//======================================================

//=================== Loop Principal ===================
void setup() {
  Serial.begin(9600);
  attachInterrupt( 2, interrupciones, CHANGE);

  pinMode (puertaOut, OUTPUT);

  //Inicializacion de los hilos
  PT_INIT(&Hilo1);
  PT_INIT(&Hilo2);

}

void loop() {
  
  digitalWrite (puertaOut, HIGH);
  
  //Temperatura();
  //Humedad();
  Read_PZEM(&Hilo1);
  //Read_Banks(&Hilo2);
  //Lectura_Puerta();
  //====== Importante =====
  //Serial.print("/");
  //Bus[0] = cambios;
  //Serial.print(Bus[0]);
  //Serial.print("/");
  //Serial.print("\r");
  //cambios = 0;
  //============
  //Combustible(); 
}
//======================================================

//====================== Funciones =====================

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

void Read_PZEM(struct pt *pt){
  PT_BEGIN(pt); //Inicio de un ProtoThread
  static long t = millis();
  static int var = 0;
  for(int i = 0; i <= 5; i++){
    Serial.print(pzems[i]);
    Serial.print("\r");
    PT_WAIT_WHILE(pt, (millis()-t)<1000);
  }
  
  var = 0;
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
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.print("&");
  Serial.print("\r");
  
  var = 0;
  Serial.print("*");
  while(var < 6){
    Serial.print(Bus_Energy[var]);
    if(var<5){
      Serial.print(",");
    }
    var++;
  }
  Serial.print("*");
  Serial.print("\r");
 PT_END(pt);
}

int Lectura_Puerta() {
  pinMode (puertaOut, OUTPUT);
  digitalWrite (puertaOut, HIGH);
  Bus[0] = digitalRead(puertaIn);
  Serial.print("+");
  Serial.print(Bus[0]);
  Serial.print("+");
  Serial.print("\r");
}

void interrupciones() {
  cambios++;
  //Serial.println("pase");
  //delay(10);
}

float Combustible() {
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

float medicionBateria() {
  float Vbat = 0;
  const int medicion = A1;
  Vbat = analogRead(medicion) * 0.004887;
  Vbat = Vbat * 6 ;
  return Vbat;
}

void ningunModulo() {
  pinMode (modulo1, OUTPUT);
  pinMode (modulo2, OUTPUT);
  pinMode (modulo3, OUTPUT);
  pinMode (modulo4, OUTPUT);
  digitalWrite (modulo1, LOW);
  digitalWrite (modulo2, LOW);
  digitalWrite (modulo3, LOW);
  digitalWrite (modulo4, LOW);
}

void Ningun_Banco() {
  for (int bat = 22; bat < 38; bat++) {
    pinMode(bat, OUTPUT);
    digitalWrite(bat, LOW);
  }
}

void Read_Banks(struct pt *pt) {
  PT_BEGIN(pt); //Inicio de un ProtoThread
  Ningun_Banco();
  static int bateria = 0;
  static long t2 = 0;
  static int var2 = 0;
  
  for (int bat = 22; bat < 26; bat++) {
    digitalWrite(bat, HIGH);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    Bus_Banco1[bateria] = medicionBateria();
    digitalWrite(bat, LOW);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    bateria ++;
  }
  var2 = 0;
  Serial.print("?");
  while(var2 < 4){
    Serial.print(Bus_Banco1[var2]);
    if(var2<3){
      Serial.print(",");
    }
    var2++;
  }
  Serial.print("?");
  Serial.print("\r");

  for (int bat = 26; bat < 30; bat++) {
    digitalWrite(bat, HIGH);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    Bus_Banco2[bateria] = medicionBateria();
    digitalWrite(bat, LOW);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    bateria ++;
  }
  var2 = 0;
  Serial.print("[");
  while(var2 < 4){
    Serial.print(Bus_Banco2[var2]);
    if(var2<3){
      Serial.print(",");
    }
    var2++;
  }
  Serial.print("[");
  Serial.print("\r");

  for (int bat = 30; bat < 34; bat++) {
    digitalWrite(bat, HIGH);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    Bus_Banco3[bateria] = medicionBateria();
    digitalWrite(bat, LOW);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    bateria ++;
  }
  var2 = 0;
  Serial.print("!");
  while(var2 < 4){
    Serial.print(Bus_Banco3[var2]);
    if(var2<3){
      Serial.print(",");
    }
    var2++;
  }
  Serial.print("!");
  Serial.print("\r");

  for (int bat = 34; bat < 38; bat++) {
    digitalWrite(bat, HIGH);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    Bus_Banco4[bateria] = medicionBateria();
    digitalWrite(bat, LOW);
    PT_WAIT_WHILE(pt, (millis()-t2)<250);
    bateria ++;
  }
  var2 = 0;
  Serial.print("]");
  while(var2 < 4){
    Serial.print(Bus_Banco4[var2]);
    if(var2<3){
      Serial.print(",");
    }
    var2++;
  }
  Serial.print("]");
  Serial.print("\r");
  PT_END(pt);
}
//======================================================

//Codificacion de Signos
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
