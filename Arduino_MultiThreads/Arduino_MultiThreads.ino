//===================== Librerias ======================
#include <SPI.h>
#include <SoftwareSerial.h>
#include <PZEM004T.h>
#include <DHT.h>
#include <pt.h>
//======================================================

//======================== Hilos =======================
struct pt Hilo1;
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
byte pzem1[] = {192, 168, 1, 1}, pzem2[] = {192, 168, 1, 2}, pzem3[] = {192, 168, 1, 3}, pzem4[] = {192, 168, 1, 4};
byte pzem5[] = {192, 168, 1, 5}, pzem6[] = {192, 168, 1, 6};

String dato[3] = {};
float bus[6] = {};
//======================================================

//=================== Loop Principal ===================
void setup() {
  Serial.begin(9600);
  attachInterrupt( 2, interrupciones, CHANGE);

}

void loop() {
  PT_INIT(&Hilo1);

}
//======================================================

//====================== Funciones =====================
float temperatura(struct pt *pt) {
  PT_BEGIN(pt); //Inicio de un ProtoThread
  bus[0] = dht1.readTemperature();
  bus[1] = dht2.readTemperature();
  bus[2] = dht3.readTemperature();
  bus[3] = dht4.readTemperature();
  PT_END(pt);

}


float humedad() {

  bus[0] = dht1.readHumidity();
  bus[1] = dht2.readHumidity();
  bus[2] = dht3.readHumidity();
  bus[3] = dht4.readHumidity();

}

float voltajeAC() {
  
  bus[0] = pzem.voltage(pzem1);
  delay(1000);
  bus[1] = pzem.voltage(pzem2);
  delay(1000);
  bus[2] = pzem.voltage(pzem3);
  delay(1000);
  bus[3] = pzem.voltage(pzem4);
  delay(1000);
  bus[4] = pzem.voltage(pzem5);
  delay(1000);
  bus[5] = pzem.voltage(pzem6);
  delay(1000);

}


float corrienteAC() {

  bus[0] = pzem.current(pzem1);
  delay(1000);
  bus[1] = pzem.current(pzem2);
  delay(1000);
  bus[2] = pzem.current(pzem3);
  delay(1000);
  bus[3] = pzem.current(pzem4);
  delay(1000);
  bus[4] = pzem.current(pzem5);
  delay(1000);
  bus[5] = pzem.current(pzem6);
  delay(1000);
}


float potenciaAC() {

  bus[0] = pzem.energy(pzem1);
  delay(1000);
  bus[1] = pzem.energy(pzem2);
  delay(1000);
  bus[2] = pzem.energy(pzem3);
  delay(1000);
  bus[3] = pzem.energy(pzem4);
  delay(1000);
  bus[4] = pzem.energy(pzem5);
  delay(1000);
  bus[5] = pzem.energy(pzem6);
  delay(1000);
}

int lecturaPuerta() {
  pinMode (puertaOut, OUTPUT);
  digitalWrite (puertaOut, HIGH);
  bus[0] = digitalRead(puertaIn);
}

void interrupciones() {
  cambios ++;
  //Serial.println("pase");
  delay(10);
}

float combustible() {
  int promedio = 0;
  for (int i = 0; i < 10; i++) {
    bus[0] = analogRead(A2);
    promedio = promedio + bus[0];
    bus[0] = promedio / (i + 1);
    delay(10);
  }
}

void enviar() {
  //if (bus[0] != 0) {
  Serial.print(bus[0]);
  //}
  //if (bus[1] != 0) {
  Serial.print(",");
  Serial.print(bus[1]);
  //}
  //if (bus[2] != 0) {
  Serial.print(",");
  Serial.print(bus[2]);
  //}
  //if (bus[3] != 0) {
  Serial.print(",");
  Serial.print(bus[3]);
  //}
  //if (bus[4] != 0) {
  Serial.print(",");
  Serial.print(bus[4]);
  //}
  //if (bus[5] != 0) {
  Serial.print(",");
  Serial.print(bus[5]);
  //}
}

void limpiarBus() {
  bus[0] = 0;
  bus[1] = 0;
  bus[2] = 0;
  bus[3] = 0;
  bus[4] = 0;
  bus[5] = 0;

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

void medirBanco() {
  ningunBanco();
  if (dato[2] == "uno") {
    int bateria = 0;
    for (int bat = 22; bat < 26; bat++) {
      digitalWrite(bat, HIGH);
      delay(250);
      bus[bateria] = medicionBateria();
      digitalWrite(bat, LOW);
      delay(250);
      bateria ++;
    }
  } else if (dato[2] == "dos") {
    int bateria = 0;
    for (int bat = 26; bat < 30; bat++) {
      digitalWrite(bat, HIGH);
      delay(250);
      bus[bateria] = medicionBateria();
      digitalWrite(bat, LOW);
      delay(250);
      bateria ++;
    }
  } else if (dato[2] == "tres") {
    int bateria = 0;
    for (int bat = 30; bat < 34; bat++) {
      digitalWrite(bat, HIGH);
      delay(250);
      bus[bateria] = medicionBateria();
      digitalWrite(bat, LOW);
      delay(250);
      bateria ++;
    }
  } else if (dato[2] == "cuatro") {
    int bateria = 0;
    for (int bat = 34; bat < 38; bat++) {
      digitalWrite(bat, HIGH);
      delay(250);
      bus[bateria] = medicionBateria();
      digitalWrite(bat, LOW);
      delay(250);
      bateria ++;
    }
  } else {
    Serial.print("banco no identificado");
  }
}

void ningunBanco() {
  for (int bat = 22; bat < 38; bat++) {
    pinMode(bat, OUTPUT);
    digitalWrite(bat, LOW);
  }
}


void modulo() {
  ningunModulo();
  if (dato[1] == "uno") {
    digitalWrite (modulo1, HIGH);
    medirBanco();
  } else if (dato[1] == "dos") {
    digitalWrite (modulo2, HIGH);
    medirBanco();
  } else if (dato[1] == "tres") {
    digitalWrite (modulo3, HIGH);
    medirBanco();
  } else if (dato[1] == "cuatro") {
    digitalWrite (modulo4, HIGH);
    medirBanco();
  } else {
    Serial.print("modulo no identificado");
  }

}
//======================================================
