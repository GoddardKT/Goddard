//Librerias
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>

//Wifi
#define WIFI_SSID "INFINITUMC7E1"    //Nombre de WIFI.
#define WIFI_PASSWORD "3YEMEE5qCY"   //PASSWORD.

//FireBase
#define FIREBASE_HOST "esp32-1-eeafc-default-rtdb.firebaseio.com"   //FireBase url.
#define FIREBASE_AUTH "mCoEtvu3R1BJoeiixKE5dbohGvnTGl4C2aH2d6mY"    //FireBase Codigo base de datos.
FirebaseData firebaseData;

//Alexa
Espalexa Alexa;



//Funciones
boolean ConectarWifi();

void alexa(uint8_t RxAlexa);

void FireBase_enviar();
void FireBase_loop();

void boton();
void AccionRele();
bool EstadoRele=0, Rele=0;
bool EstadoW=true;


void setup() {
  pinMode(2,INPUT_PULLUP);             //Pin 2 para boton.
  pinMode(0,OUTPUT);            //Pin 0 es para rele.

  ConectarWifi();               //Inicio conexión WIFI

  
  if (ConectarWifi){            //Conectar a base de datos.
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
                                //Nombre de dispositivo para alexa
    Alexa.addDevice("Prueba1", alexa);           
    }
  else{
    //Serial.println("No Conectado de base de datos...");
    }
    
    Alexa.begin();
}

void loop() {
  boton();
  Alexa.loop();
  FireBase_loop();
  
}


void boton(){
  if (digitalRead(2)==0){
    delay(200);
    EstadoRele=!EstadoRele;
  }
  AccionRele();
}

void AccionRele(){
 if (EstadoRele==0){
  digitalWrite(0,0);
 }
 else{
  digitalWrite(0,1);
 }
  FireBase_enviar();
}


void alexa(uint8_t RxAlexa){
  if (RxAlexa == 255) {
      EstadoRele=0;
    }
    else if (RxAlexa == 0) {
      EstadoRele=1;
    }
    AccionRele();
}


void FireBase_enviar(){
  if(EstadoRele==1){
    Firebase.setBool(firebaseData,"/Led1Status",true);
  }
  else if(EstadoRele==0){
    Firebase.setBool(firebaseData,"/Led1Status",false);
  }
}


void FireBase_loop(){
  if (EstadoW==1){ 
      Firebase.getBool(firebaseData, "/Led1Status");
      bool EstadoFB = firebaseData.boolData();
      if (EstadoRele!=EstadoFB){
          EstadoRele=EstadoFB;
          AccionRele();
        }
    }
    
}











boolean ConectarWifi(){
  //bool estadoW = true;                     //Variable para saber estado de wifi
  int i = 0;
  
  WiFi.mode(WIFI_STA);   //Modo estación
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   //Cargar nombre y contraseña

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 50){
      EstadoW = false; break;
    }
    i++;
  }
 // Serial.println("");
  if (EstadoW){
    /*Serial.print("Conectado a ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Dirección: ");
    Serial.println(WiFi.localIP());*/
    digitalWrite(0,1);
    delay(500);
    digitalWrite(0,0);
    delay(500);
    digitalWrite(0,1);
    delay(500);
    digitalWrite(0,0);
    delay(500);
  }
  else {
    digitalWrite(0,1);//Coneción fallida
  }
  return EstadoW;
}
