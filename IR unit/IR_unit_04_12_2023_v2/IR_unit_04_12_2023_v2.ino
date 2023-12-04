#define IR_LED 5

void setup() {
  Serial.begin(115200);
  initialize_IR_module();
}



uint8_t a = 0;
void loop() {
  transmit_preamble();
  transmit_zero();
  transmit_one();
  transmit_trigger();
  delay(10);
}
