void setup() {
  Serial.begin(115200);
  initialize_IR_module();

  uint8_t data[]= {
    1,1,1,1,0,1,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,0,1,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1
    };

  for(uint8_t i = 0; i< 40 ; i++){
    set_IR_module_buffer(i,data[i]);
  }

}

void loop() {
  receive_package_if_possible();
  //transmit_package();
  //delay(25);

}