void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(2)){
      Serial.println("activo");
      delay(1000);
    }
  else{
    Serial.println("inactativo");
    delay(1000);
    }
}
