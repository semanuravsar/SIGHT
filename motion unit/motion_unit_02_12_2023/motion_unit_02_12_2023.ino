
void setup() {
Serial.begin(115200);
initialize_motor_pins();
}
void loop() {
  //test_measure_battery_voltage(0.333);
  //test_motor_wo_delay();
  drive_right_motor(255);
}