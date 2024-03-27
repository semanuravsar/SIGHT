
#include <Wire.h>
//control pins for left and right motors
const int leftSpeed = 9; //means pin 9 on the Arduino controls the speed of left motor
const int rightSpeed = 10;
const int left1 = 7; //left 1 and left 2 control the direction of rotation of left motor
const int left2 = 8;
const int right1 = 2;
const int right2 = 4;

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ; //linear acceleration
float GyroX, GyroY, GyroZ; //angular velocity
float GyroZReal; //used in void loop()
float biasZ;
float GyroZAcc;
float elapsedTime, currentTime, previousTime;
float yaw;
int angle;
int initialAngle = 0;
int targetAngle = 50;
int error;
float previousError = 0;
float accError;
float derivative;
int integralLimit = 100;
float controlOutput, Kp, Ki, Kd;
int baseSpeed=127;
int leftSpeedVal = baseSpeed;
int  rightSpeedVal = baseSpeed;
const int maxSpeed = 196; //max PWM value written to motor speed pin. It is typically 255.
const int minSpeed = 64;
int c = 0;
 //min PWM value at which motor moves
 

  void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);
  caliber();
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(leftSpeed, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  delay(20);
  currentTime = micros();

}
bool isStationary(float thresholdGyro = 2.0) {
    static unsigned long stationaryStartTime = 0; // Holds the start time when the robot first became stationary
    unsigned long stationaryDuration = 20; // Duration in milliseconds to consider the robot truly stationary for recalibration
    unsigned long stationaryTimePassed;
    readGyro(); // Assume this updates GyroX, GyroY, GyroZ
    

    
     // Assuming 1g = stationary, adjust for orientation

    // Check if the robot is nearly still
    if (abs(GyroZReal) < thresholdGyro) {
        if (stationaryStartTime == 0) { // Not previously stationary
            stationaryTimePassed = millis(); // Start the timer
        } else if (stationaryTimePassed - stationaryStartTime >= stationaryDuration) { // Has been stationary long enough
            return true;
        }
    } else {
        stationaryStartTime = 0; // Reset timer if moving
    }
    return false;
}
  void loop() {
  // put your main code here, to run repeatedly:
  readGyro(); // Continues to update gyro readings
    if (isStationary()) { // Checks if the robot has been stationary for the specified duration
        caliber(); // Performs recalibration if stationary
    }
    GyroZReal = GyroZ - biasZ;
  // finds time
  previousTime = currentTime;  
  currentTime = micros();
  elapsedTime = (currentTime - previousTime) / 1000000;
  if (abs(GyroZ) > 2){
      yaw += + GyroZReal * elapsedTime;
  }
  angle = initialAngle + (round(yaw)) % 360;
  
  controller();
  Serial.print("angle ;");
  Serial.println(angle);

  rotate();
}



  void controller(){
  // coefficients
  Kp = 1;
  Ki = 0.01;
  Kd = 0;
    // angle difference calculation
  error = targetAngle - angle;
  derivative = (error - previousError)/elapsedTime;
   //integral of the error
  accError += error * elapsedTime;
  // anti windup: limit the integral term

  controlOutput = Kp*error + Ki*accError;
  previousError = error;


}
  void caliber(){

  int count = 0;
  while (count < 200){
    readGyro();
    GyroZAcc += GyroZ;
    count += 1;
  biasZ = GyroZAcc/200;
  }

}

  void readGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
}


  void rotate() {
    if (abs(error) < 3){
      stop();
    } else {
    if (error < -3) { //turn left
      right();
      leftSpeedVal = changeSpeed(leftSpeedVal, +1);
      rightSpeedVal =  changeSpeed(rightSpeedVal, -1);
      
    } else if (error > 3) {//turn right
      left();
      leftSpeedVal =  changeSpeed(leftSpeedVal, -1);
      rightSpeedVal = changeSpeed(rightSpeedVal, +1);
    }
    analogWrite(rightSpeed, rightSpeedVal);
    analogWrite(leftSpeed, leftSpeedVal);

      
      
    } 
      
    }
    





  int changeSpeed (int motorSpeed, int increment){
  motorSpeed += increment;
  if (motorSpeed > maxSpeed){ //to prevent motorSpeed from exceeding 255, which is a problem when using analogWrite
    motorSpeed = maxSpeed;
  } else if (motorSpeed < minSpeed){
    motorSpeed = minSpeed;
  }
  return motorSpeed;
}

  void stop(){
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  analogWrite(rightSpeed, 0);
  analogWrite(leftSpeed, 0);
}

void forward(){ //drives the car forward, assuming leftSpeedVal and rightSpeedVal are set high enough
  digitalWrite(right1, HIGH); //the right motor rotates FORWARDS when right1 is HIGH and right2 is LOW
  digitalWrite(right2, LOW);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void left(){ //rotates the car left, assuming speed leftSpeedVal and rightSpeedVal are set high enough
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void right(){
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
}




