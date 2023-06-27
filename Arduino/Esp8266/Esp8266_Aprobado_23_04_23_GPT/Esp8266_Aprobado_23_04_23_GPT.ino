#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>

#define WIFI_SSID "INFINITUMC7E1"
#define WIFI_PASSWORD "3YEMEE5qCY"

#define FIREBASE_HOST "esp32-1-eeafc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "mCoEtvu3R1BJoeiixKE5dbohGvnTGl4C2aH2d6mY"

FirebaseData firebaseData;
Espalexa Alexa;

bool EstadoRele = 0;
bool EstadoW = true;

void ConectarWifi();
void FireBase_enviar();
void AccionRele();
void boton();
void alexa(uint8_t RxAlexa);
void FireBase_loop();

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(0, OUTPUT);
  ConectarWifi();
  if (EstadoW) {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Alexa.addDevice("Prueba1", alexa);
  }
  Alexa.begin();
}

void loop() {
  boton();
  //Alexa.loop();
  FireBase_loop();
}

void boton() {
  if (digitalRead(2) == 0) {
    delay(200);
    EstadoRele = !EstadoRele;
    AccionRele();
  }
}

void AccionRele() {
  digitalWrite(0, EstadoRele);
  FireBase_enviar();
}

void alexa(uint8_t RxAlexa) {
  EstadoRele = RxAlexa == 0;
  AccionRele();
}

void FireBase_enviar() {
  Firebase.setBool(firebaseData, "/Led1Status", EstadoRele);
}

void FireBase_loop() {
  if (EstadoW) {
    Firebase.getBool(firebaseData, "/Led1Status");
    bool EstadoFB = firebaseData.boolData();
    if (EstadoRele != EstadoFB) {
      EstadoRele = EstadoFB;
      AccionRele();
    }
  }
}

void ConectarWifi() {
  int i = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (i > 50) {
      EstadoW = false;
      break;
    }
    i++;
  }
  if (EstadoW) {
    digitalWrite(0, 1);
    delay(500);
    digitalWrite(0, 0);
    delay(500);
    digitalWrite(0, 1);
    delay(500);
    digitalWrite(0, 0);
    delay(500);
  }
  else {
    digitalWrite(0, 1);
  }
}
