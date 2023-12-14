
void setup() {
  Serial.begin(115200);
  initialize_IR_module();
}



uint8_t a = 0;
void loop() {

listen_IR();

}
