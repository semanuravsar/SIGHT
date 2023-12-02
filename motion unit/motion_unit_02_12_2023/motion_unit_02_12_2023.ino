
void setup() {
Serial.begin(115200);
initialize_motor_module();
}


void loop() {

  Serial.println(get_max_pwm_allowed());
  drive_right_motor(300);
}


