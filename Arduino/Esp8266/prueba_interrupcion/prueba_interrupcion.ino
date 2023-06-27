//Librerias
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>

//___________________________________________________________________________________________________________________________________
//Wifi
#define WIFI_SSID "TP-Link_6070"    //Nombre de WIFI.
#define WIFI_PASSWORD "84867988"    //PASSWORD.

//FireBase
#define FIREBASE_HOST "esp32-1-eeafc-default-rtdb.firebaseio.com"   //FireBase url.
#define FIREBASE_AUTH "mCoEtvu3R1BJoeiixKE5dbohGvnTGl4C2aH2d6mY"    //FireBase Codigo base de datos.
FirebaseData firebaseData;

//Alexa
Espalexa Alexa;
//___________________________________________________________________________________________________________________________________

#define Rele 0
//___________________________________________________________________________________________________________________________________
boolean ConectarWifi();

void boton();

void alexa(uint8_t RxAlexa);

void FireBase_enviar();
void FireBase_loop();

void rele();

//___________________________________________________________________________________________________________________________________
bool estadoR=0, estadoFB=0, estadoW=1;
int flag=0;
//___________________________________________________________________________________________________________________________________



void setup() {
  pinMode(2,INPUT_PULLUP);      //Pin 2 para boton.
  pinMode(Rele,OUTPUT);            //Pin 0 es para rele.

  ConectarWifi();         //Inicio conexión WIFI

  //Conectar a base de datos.
  if (ConectarWifi){
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    //Serial.println("Conectado de base de datos...");

    Alexa.addDevice("Prueba2", alexa);
    
    }
  else{
    //Serial.println("No Conectado de base de datos...");
    }
    
    Alexa.begin();
    
}

void loop() {
  Alexa.loop();
  boton();
  FireBase_loop();

  /*switch (flag){
  case 1:{//boton
     rele();
     break;
    }
  case 2:{//alexa
     rele();
     break;
    }
  case 3:{//FireBase
     rele();
     break;
    }
  }*/
}

void boton(){
  if (digitalRead(2)==0){
    estadoR=!estadoR;
    delay(200);
    rele();
    flag=1;
  }
}

void alexa(uint8_t RxAlexa){
    if (RxAlexa == 255) {
      estadoR=true;
    }
    else if (RxAlexa == 0) {
      estadoR=false;
    }
    rele();
    flag=2;
}


void rele(){
  if (estadoR==0){
      digitalWrite(0,1);
      Firebase.setBool(firebaseData,"/Led1Status",false);
    }
  else{
       digitalWrite(0,0);
       Firebase.setBool(firebaseData,"/Led1Status",true);
    }
    flag=0;
}


void FireBase_loop(){
  if (estadoW==1){
      Firebase.getBool(firebaseData, "/Led1Status");
      estadoFB = firebaseData.boolData();
      if (estadoR!=estadoFB){
          estadoR=estadoFB;
          rele();
          flag=3;
        }
    }
}




boolean ConectarWifi(){
  digitalWrite(0,1);
    delay(500);
    digitalWrite(0,0);
    delay(500);
    digitalWrite(0,1);
    delay(500);
    digitalWrite(0,0);
    delay(500);
    
  estadoW = true;  //Variable para saber estado de wifi
  int i = 0;
  
  WiFi.mode(WIFI_STA);   //Modo estación
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   //Cargar nombre y contraseña
  //Serial.println("");
 //Serial.println("Conectar WiFi");

  // Esperando conexión.
  //Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 50){
      estadoW = false; break;
    }
    i++;
  }
 // Serial.println("");
  if (estadoW){
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
    //Serial.println("Coneción fallida.");
    digitalWrite(0,1);
  }
  return estadoW;
}
