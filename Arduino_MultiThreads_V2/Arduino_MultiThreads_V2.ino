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
//======================================================

//====================== Pointers ======================
byte ipPZEM[6][4] = {{192, 168, 1, 1}, {192, 168, 1, 2}, {192, 168, 1, 3}, {192, 168, 1, 4}, {192, 168, 1, 5}, {192, 168, 1, 6}};
//======================================================

//=================== Loop Principal ===================
void setup() {
  Serial.begin(9600);
  attachInterrupt( 2, Interrupciones, CHANGE);

  pinMode (puertaOut, OUTPUT);

}

void loop() {
  Temperatura();
  Humedad();
  Read_PZEM();

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
void Read_PZEM(){
  int* tempIP = new int(9);
  for(int i = 0; i < 6; i++){
    for(int n = 0; n < 4; n++){
      tempIP[n] = ipPZEM[i][n];
    }
    Serial.print(*tempIP);
    Serial.print("\r");
    delete [] tempIP;
  }
}
//======================================================
