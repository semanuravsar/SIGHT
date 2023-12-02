
void setup() {
Serial.begin(115200);
initialize_motor_pins();
startTime = millis(); // Record the start time
}
void loop() {
  //test_measure_battery_voltage(0.333);
  test_motor();
}