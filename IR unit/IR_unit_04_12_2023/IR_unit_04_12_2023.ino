void setup() {
  Serial.begin(115200);
  initialize_IR_module();

}

void loop() {
  receive_package_if_possible();
  //transmit_byte_package(32);
  //delay(1000);

}