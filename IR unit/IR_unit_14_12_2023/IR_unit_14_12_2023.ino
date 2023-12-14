
void setup() {
  Serial.begin(115200);
  initialize_IR_module();
}



uint8_t a = 0;
void loop() {
  set_buffer(0, 34);
  set_buffer(1, 34);
  set_buffer(2, 34);
  set_buffer(3, 34);
  transmit_buffer();
  //listen_IR();
  delay(50);
}
