#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>


#define WIFI_SSID "TP-Link_6070"    //Nombre de WIFI.
#define WIFI_PASSWORD "84867988"    //PASSWORD.

#define FIREBASE_HOST "esp32-1-eeafc-default-rtdb.firebaseio.com"   //FireBase url.
#define FIREBASE_AUTH "mCoEtvu3R1BJoeiixKE5dbohGvnTGl4C2aH2d6mY"    //FireBase Codigo base de datos.
FirebaseData firebaseData;

Espalexa Alexa;

//funciones
boolean ConectarWifi();   //Funcion para conectar WIFI.

void FocoSala(uint8_t brightness);

bool i=true;
bool estadoFB=false;

void onoff();

void setup() {
  pinMode(0,OUTPUT);      //Pin 0 es LED de placa.
  pinMode(2,INPUT_PULLUP);      //Pin 2 es LED de placa.
  //Inicializa
  Serial.begin(115200);   //Inicio comunicación serial
  ConectarWifi();         //Inicio conexión WIFI

  //Conectar a base de datos.
  if (ConectarWifi){
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Serial.println("Conectado de base de datos...");

    Alexa.addDevice("ESP8266", FocoSala);
    
    }
  else{
    Serial.println("No Conectado de base de datos...");
    }
    
    Alexa.begin();
    Firebase.getBool(firebaseData, "/Led1Status");
    estadoFB = firebaseData.boolData();

    attachInterrupt(digitalPinToInterrupt(2),onoff, FALLING);
}

void onoff(){
  bool estadoINT=true;
  
  while(estadoINT){
   Firebase.getBool(firebaseData, "/Led1Status");
   estadoFB = firebaseData.boolData();

   if (estadoFB){
      estadoFB=false;
      Firebase.setBool(firebaseData,"/Led1Status",false);
      digitalWrite(0,1);
    }
   else{
      estadoFB=true;
      Firebase.setBool(firebaseData,"/Led1Status",true);
      digitalWrite(0,0);
    }

    estadoINT=false;
    delay(2000);
  }
}



void loop() {
  Alexa.loop();
  Firebase.getBool(firebaseData, "/Led1Status");
  estadoFB = firebaseData.boolData();
  if(estadoFB== true){
    Serial.println("on1");
    digitalWrite(0,0);
    } 
  else {
    Serial.println("off1");
    digitalWrite(0,1);
    }
}

//Funciones -------------------------------------------------------------------------------------------------------------------------

boolean ConectarWifi(){
  boolean estadoW = true;  //Variable para saber estado de wifi
  int i = 0;
  
  WiFi.mode(WIFI_STA);   //Modo estación
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   //Cargar nombre y contraseña
  Serial.println("");
  Serial.println("Conectar WiFi");

  // Esperando conexión.
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      estadoW = false; break;
    }
    i++;
  }
  Serial.println("");
  if (estadoW){
    Serial.print("Conectado a ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Dirección: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Coneción fallida.");
  }
  return estadoW;
}


void FocoSala(uint8_t brightness){
  Serial.print("Luces Sala Cambio a:");
     
    if (brightness == 255) {
      Serial.print("Encendido");
      Firebase.setBool(firebaseData,"/Led1Status",true);
    }
    
    else if (brightness == 0) {
      Serial.print("Apagado");
      Firebase.setBool(firebaseData,"/Led1Status",false);
    }
}
