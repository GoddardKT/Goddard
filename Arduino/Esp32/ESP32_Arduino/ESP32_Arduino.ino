#include <FirebaseESP32.h>
#include <WiFi.h>

#define WIFI_SSID "TP-Link_6070" // your wifi SSID
#define WIFI_PASSWORD "84867988" //your wifi PASSWORD

#define LedPin 2         // pin d0 as toggle pin
#define FIREBASE_HOST "esp32-1-eeafc-default-rtdb.firebaseio.com" // change here
#define FIREBASE_AUTH "mCoEtvu3R1BJoeiixKE5dbohGvnTGl4C2aH2d6mY"// change here
FirebaseData firebaseData;

void setup ()
{
  pinMode(LedPin, OUTPUT);
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);     
}
void loop (){ 
 if(Firebase.getString(firebaseData, "/Led1Status")){
    String ledstatus1 = firebaseData.stringData();
    if(ledstatus1.toInt() == 1){
      Serial.println("on1");
        digitalWrite(LedPin,1);
    }
    else {
      Serial.println("off1");
        digitalWrite(LedPin,0);
    }
 }
}
