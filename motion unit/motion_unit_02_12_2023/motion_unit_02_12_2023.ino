
void setup() {
Serial.begin(115200);
initialize_motor_module();
}

void loop() {
  validator_validate_motion_module();
}


