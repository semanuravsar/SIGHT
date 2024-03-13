#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

// Define motor control pins
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 9  // Ensure this pin supports PWM
#define ENB 10 // Ensure this pin supports PWM

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  } else {
    Serial.println("MPU6050 connection successful");
  }

  // Initialize motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Simple control: Use gx to control speed
  int speed = map(gz, -32768, 32767, 0, 255); // Map gyro range to speed range
  speed = constrain(speed, 0, 255); // Constrain speed to valid PWM range

  // Set motor direction to forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  // Apply speed control
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  Serial.print("Gyro Z: "); Serial.print(gz); 
  Serial.print(" | Speed: "); Serial.println(speed);

  delay(100); // Update at 10Hz
}
