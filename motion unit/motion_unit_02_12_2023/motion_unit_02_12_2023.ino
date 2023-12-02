
void setup() {
Serial.begin(115200);
initialize_motor_module();
Serial.println(get_max_pwm_allowed());
}

unsigned long last_time_zaxd = 0;
int desired_pwmmmm = -200;
void loop() {
  if(millis()-last_time_zaxd >5000){
    desired_pwmmmm += 50;
    last_time_zaxd = millis();
  }

  drive_right_motor(desired_pwmmmm);
  drive_left_motor(desired_pwmmmm);
  //test_H_bridge_fully_on();
}


