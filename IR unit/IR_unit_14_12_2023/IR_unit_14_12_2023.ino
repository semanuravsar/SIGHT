
void setup() {
  Serial.begin(115200);
  initialize_IR_module();
}



uint8_t a = 0;
void loop() {
  transmit_zero();
  transmit_zero();
  transmit_one();
  transmit_zero();

  transmit_zero();
  transmit_zero();
  transmit_one();
  transmit_zero();

  transmit_zero();
  transmit_zero();
  transmit_one();
  transmit_zero();

  transmit_zero();
  transmit_zero();
  transmit_one();
  transmit_zero();

  delay(100);
}
