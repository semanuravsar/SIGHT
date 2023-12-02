
void setup() {
Serial.begin(115200);
initialize_motor_module();
Serial.println(get_max_pwm_allowed());
}

unsigned long last_time_zaxd = 0;
int desired_pwmmmm = 0;
void loop() {
  if(millis()-last_time_zaxd >30){
    desired_pwmmmm += 1;
    last_time_zaxd = millis();
    Serial.println(desired_pwmmmm);

  }

  drive_right_motor(desired_pwmmmm);
  //test_H_bridge_fully_on();
}


