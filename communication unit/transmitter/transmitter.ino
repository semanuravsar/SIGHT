void setup() {
 pinMode(8, OUTPUT);

}

void loop() {
  for(uint8_t i=0; i < 50; i++){
      digitalWrite(8,HIGH);
      delayMicroseconds(10);
      digitalWrite(8,LOW);
      delayMicroseconds(10);

  }
  delay(1000);

}
