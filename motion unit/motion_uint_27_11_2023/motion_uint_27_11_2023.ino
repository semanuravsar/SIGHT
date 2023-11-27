

void setup() {
  pinMode(8, OUTPUT);   //IN2
  pinMode(9, OUTPUT);   //IN1
  pinMode(10, OUTPUT);  //Enable Pin (IN1 & IN2)
  pinMode(6, OUTPUT);   // IN3
  pinMode(7, OUTPUT);   //IN4
  pinMode(11, OUTPUT);  // Enable Pin (IN3 & IN4)
  delay(3000);
}
void loop() {

  //forward motion
  for (int i = 25; i < 80; i++) {
    right_forward(i);
    left_forward(i);
    delay(30);
  }
  delay(1500);
  for (int i = 80; i > 0; i--) {
    right_forward(i);
    left_forward(i);
    delay(10);
  }

  // Backward motion
  for (int i = 25; i < 125; i++) {
    right_backward(i);
    left_backward(i);
    delay(30);
  }
  delay(1500);
  for (int i = 125; i > 0; i--) {
    right_backward(i);
    left_backward(i);
    delay(10);
  }
  // rotate around itself
  for (int i = 25; i < 80; i++) {
    right_forward(i);
    left_backward(i);
    delay(30);
  }
  for (int i = 80; i > 0; i--) {
    right_forward(i);
    left_backward(i);
    delay(30);
  }
  delay(5000);
}


void right_backward(uint8_t pwm_val) {
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  analogWrite(10, pwm_val);
}


void right_forward(uint8_t pwm_val) {
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  analogWrite(10, pwm_val);
}

void left_forward(uint8_t pwm_val) {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  analogWrite(11, pwm_val);
}

void left_backward(uint8_t pwm_val) {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  analogWrite(11, pwm_val);
}
