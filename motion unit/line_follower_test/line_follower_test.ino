uint8_t analog_pins[6] = {A0, A1, A2, A3, A4, A5}; // Define the analog input pins
#define rightMotor1 13
#define rightMotor2 12
#define rightMotorPWM 11
#define leftMotor1 8
#define leftMotor2 7
#define leftMotorPWM 10
/*  digitalWrite(rightMotor1,HIGH);
    digitalWrite(rightMotor2, LOW); right motor forward

    digitalWrite(leftMotor1,LOW);
    digitalWrite(leftMotor2, HIGH); left motor forward
*/

void setup() {
  Serial.begin(9600);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);

  for (uint8_t i = 0; i < 6; i++) {
    pinMode(analog_pins[i], INPUT);
  }  
}

unsigned int threshold = 710; // Threshold for detecting black
int baseSpeed = 100; // Base speed of the robot
int error = 0;
int correction = 0;

void loop() {
  int sensorValues[6];
  int activeSensors = 0;
  error = 0;

  // Read sensor values and determine position error
  for (uint8_t i = 0; i < 6; i++) {
    sensorValues[i] = analogRead(analog_pins[i]);
    if (sensorValues[i] > threshold) {
      error += (i - 2.5); // Calculate position error assuming sensor 3 is the center
      activeSensors++;
    }
  }

  // Calculate correction based on proportional control
  if (activeSensors > 0) {
    error /= activeSensors; // Average error across active sensors
  }
  correction = error * 10; // Proportional gain

  // Adjust motor speeds based on correction
  int leftMotorSpeed = baseSpeed - correction;
  int rightMotorSpeed = baseSpeed + correction;

  // Function to set motor speeds (to be implemented)
  setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

  // Print sensor values and control data
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValues[i]);
  }
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" Correction: ");
  Serial.println(correction);
  Serial.print("Left Motor: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" Right Motor: ");
  Serial.println(rightMotorSpeed);

  delay(100);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Code to control motor speeds
  digitalWrite(rightMotor1,HIGH);
    digitalWrite(rightMotor2, LOW);

    digitalWrite(leftMotor1,LOW);
    digitalWrite(leftMotor2, HIGH);
   analogWrite(leftMotorPWM, leftSpeed);
   analogWrite(rightMotorPWM, rightSpeed);
}
