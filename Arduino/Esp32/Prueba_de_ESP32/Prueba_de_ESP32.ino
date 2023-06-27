//Placa ESP32 Dev Module
void setup()
{
 Serial.begin(115200);        //Inicia Serial 115200 baudios
 pinMode(2,OUTPUT);           //Pin 2 en modo salida
 pinMode(0,OUTPUT);           //Pin 2 en modo salida

}
void loop()
{
 Serial.println("Goddard");   //Enviar por serial "Goddard"
 
 digitalWrite(2,1);           //Pone en alto pin 2
 digitalWrite(0,1);           //Pone en alto pin 2

 delay(2000);                 //Espera 1s
 digitalWrite(2,0);           //Pone en bajo pin 2
 digitalWrite(0,0);           //Pone en alto pin 2

 delay(2000);  
}
