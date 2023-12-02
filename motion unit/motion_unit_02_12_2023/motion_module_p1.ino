#define in_1 8    //IN1
#define in_2 9    //IN2
#define en_12 10  //Enable Pin (IN1 & IN2)
#define in_3 6    //IN3
#define in_4 7    //IN4
#define en_34 11  //Enable Pin (IN3 & IN4)
#define builtin_LED 13
#define voltage_reading_pin A0

//Global variables
const int MOTOR_RATED_VOLTAGE = 5;     //by default  [volt]
float BATTERY_VOLTAGE = 6;             //by default  [volt]
int MAX_PWM_ALLOWED = 255;             //by default  [0,255]
unsigned int PWM_STEP_SIZE = 1;        //by default  [0,255]
unsigned int PWM_UPDATE_PERIOD = 250;  //by default  [miliseconds]

//=================================================
void initialize_motor_module() {
  //The LED blinking is put here so that we can detect reboots. May be removed in the future.
  pinMode(builtin_LED, OUTPUT);
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(builtin_LED, HIGH);
    delay(100);
    digitalWrite(builtin_LED, LOW);
    delay(100);
  }
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(en_12, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);
  pinMode(en_34, OUTPUT);
  pinMode(voltage_reading_pin, INPUT);

  set_max_pwm_allowed();

  delay(100);  // While not strictly necessary, it's considered good practice to include this delay after initialization.
}

void haltMotorModule() {
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, LOW);
  digitalWrite(en_12, LOW);
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, LOW);
  digitalWrite(en_34, LOW);
}

int get_max_pwm_allowed() {
  return MAX_PWM_ALLOWED;
}
int set_max_pwm_allowed() {
  MAX_PWM_ALLOWED = floor((MOTOR_RATED_VOLTAGE / BATTERY_VOLTAGE) * 255);  //do not use round because it may round 255.X to 256. Which is not expected by the analog write
}

void drive_right_motor(int pwm_aimed) {
  static int pwm_current = 0;
  static unsigned long updateTime = 0;

  unsigned long currentTime = millis();                  // Get the current time since the arduino starts in miliseconds
  unsigned long elapsedTime = currentTime - updateTime;  // Calculate elapsed time in miliseconds
  if (elapsedTime >= PWM_UPDATE_PERIOD) {

    // Update pwm current ======================================
    int pwm_difference = pwm_aimed - pwm_current;
    uint8_t pwm_change = 0;
    if (pwm_difference > 0) {  //increase PWM
      pwm_current = min(PWM_STEP_SIZE, pwm_difference);
    } else if (pwm_difference < 0) {  //decrease PWM
      pwm_current -= min(PWM_STEP_SIZE, abs(pwm_difference));
    }

    // drive right motor =======================================
    if ((pwm_aimed < -MAX_PWM_ALLOWED) || (MAX_PWM_ALLOWED < pwm_aimed)) {
      // If the index exceeds the expected range, it's a critical error. Users of this function must ensure the input falls within the range of [-MAX_PWM_ALLOWED, MAX_PWM_ALLOWED] and is an integer.
      // In case of an error, the algorithm is halted, and the built-in LED blinks a message in Morse code to notify the user :)
      for (int i = 0; i < 1000; i++) {
        haltMotorModule();  //TODO: When combined with other modules, this function should stop all other modules to
        stopped_hearth_sound();   // "HELP" in morse code :)
      }
    } else if ((0 <= pwm_current) && (pwm_current <= MAX_PWM_ALLOWED)) {
      // forward motion
      Serial.println("forward");
      digitalWrite(in_1, HIGH);
      digitalWrite(in_2, LOW);
      analogWrite(en_12, pwm_current);

    } else if ((-MAX_PWM_ALLOWED <= pwm_current) && (pwm_current < 0)) {
      // "backward motion
      Serial.println("backward");
      digitalWrite(in_1, LOW);
      digitalWrite(in_2, HIGH);
      analogWrite(en_12, -pwm_current);
    }

    //update the updateTime
    updateTime = currentTime;
  }
}
void stopped_hearth_sound() {
  const int dotDelay = 90;  // Time for a dot in milliseconds
  const int silenceDelay = dotDelay / 5;
  const int wordDelay = 10 * dotDelay;  // Time between words is 7 times the dot delay

  // Define arrays to store the durations of sounds and silences
  const int soundDurations[] = { 3 * dotDelay, 1 * dotDelay, 3 * dotDelay, 1 * dotDelay,  3 * dotDelay,1 * dotDelay, 30 * dotDelay };
  const int silenceDurations[] = { 5 * dotDelay, 5 * dotDelay, 5 * dotDelay, 5 * dotDelay, 5*dotDelay, 5 * dotDelay,5 * dotDelay };

  for (int i = 0; i < 7; i++) {  // Loop through the hardcoded Morse code pattern
    digitalWrite(builtin_LED, HIGH);
    delay(soundDurations[i]);
    digitalWrite(builtin_LED, LOW);
    delay(silenceDurations[i]);

    delay(silenceDelay);  // Time between dots and dashes within the same letter
  }

  delay(wordDelay);  // Time between words
}
//=================================================

void test_initialize_motor_pins() {
  pinMode(builtin_LED, OUTPUT);
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(builtin_LED, HIGH);
    delay(250);
    digitalWrite(builtin_LED, LOW);
    delay(250);
  }
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(en_12, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);
  pinMode(en_34, OUTPUT);
  pinMode(voltage_reading_pin, INPUT);
  delay(1000);
}

void test_measure_battery_voltage(float reduction_ratio) {
  Serial.println("\nreading battery voltage with reduction ratio of: " + String(reduction_ratio));
  int reading = analogRead(voltage_reading_pin);
  Serial.println("reading is: " + String(reading) + " / 1023");
  float actual_voltage = (5.0 * reading / 1024.0) / reduction_ratio;
  Serial.println("Actual battery voltage is: " + String(actual_voltage) + " V");
}

int test_set_max_pwm_allowed(float battery_voltage) {
  float motor_rated_voltage = 6;
  int max_pwm_allowed = 0;
  if (battery_voltage > 0) {
    Serial.println("\n Calculating max-pwm allowed for battery voltage of " + String(battery_voltage));
    max_pwm_allowed = floor((motor_rated_voltage / battery_voltage) * 255);  //do not use round because it may round 255.X to 256. Which is not expected by the analog write
    Serial.println("Max pwm is calculated as " + String(max_pwm_allowed));
  } else {
    Serial.println("\n Could not calculate max-pwm allowed for battery voltage of " + String(battery_voltage));
  }
  return max_pwm_allowed;
}

void test_motor_wo_delay() {
  static int step_size = 1;
  static int pwm_current = 0;
  static int pwm_aimed = 255;
  static unsigned long updateTime = 0;
  static int direction_flag = 1;  //1 forward, 0 backward

  int deltaT = 25;

  int battery_voltage = 6;
  float rated_pwm_max = test_set_max_pwm_allowed(battery_voltage);

  unsigned long currentTime = millis();                  // Get the current time
  unsigned long elapsedTime = currentTime - updateTime;  // Calculate elapsed time
  if (elapsedTime >= deltaT) {

    int pwm_difference = pwm_aimed - pwm_current;
    if (pwm_difference > 0) {  //increase PWM
      pwm_current += min(step_size, pwm_difference);
    } else if (pwm_difference < 0) {  //decrease PWM
      pwm_current -= min(step_size, abs(pwm_difference));
    } else {
      if (direction_flag == 1) {
        pwm_aimed = -255;
        direction_flag = 0;
      } else {
        pwm_aimed = 255;
        direction_flag = 1;
      }
    }

    test_drive_right_motor(pwm_current, rated_pwm_max);
    test_drive_left_motor(pwm_current, rated_pwm_max);
    updateTime = elapsedTime;
  }
}

void test_motor() {
  delay(5000);
  int pwm_i = 0;
  int step_size = 1;
  int pwm_min = -256;
  int pwm_max = 256;
  int deltaT = 25;
  int battery_voltage = 12;

  float rated_pwm_max = test_set_max_pwm_allowed(battery_voltage);

  int current_pwm = pwm_i;
  Serial.println("\ntesting starts");
  Serial.println("\ndecrease current pwm");
  for (int i = current_pwm; i > pwm_min; i = i - step_size) {
    test_drive_right_motor(i, rated_pwm_max);
    test_drive_left_motor(i, rated_pwm_max);
    current_pwm = i;
    delay(deltaT);
  }
  Serial.println("\nincrease current pwm");
  for (int i = current_pwm; i < pwm_max; i = i + step_size) {
    test_drive_right_motor(i, rated_pwm_max);
    test_drive_left_motor(i, rated_pwm_max);
    current_pwm = i;
    delay(deltaT);
  }
  Serial.println("\ndecrease current pwm");
  for (int i = current_pwm; i > 0; i = i - step_size) {
    test_drive_right_motor(i, rated_pwm_max);
    test_drive_left_motor(i, rated_pwm_max);
    current_pwm = i;
    delay(deltaT);
  }
  Serial.println("\ntesting ends");
}

void test_drive_right_motor(int pwm_val, int max_pwm_allowed) {
  Serial.println("\ncalling test_drive_right_motor() with pwm_val of:" + String(pwm_val));
  if ((0 <= pwm_val) && (pwm_val <= max_pwm_allowed)) {
    digitalWrite(in_1, HIGH);
    digitalWrite(in_2, LOW);
    analogWrite(en_12, pwm_val);
    Serial.println("forward motion");
  } else if ((-max_pwm_allowed <= pwm_val) && (pwm_val < 0)) {
    digitalWrite(in_1, LOW);
    digitalWrite(in_2, HIGH);
    analogWrite(en_12, -pwm_val);
    Serial.println("backward motion");
  } else {
    digitalWrite(in_1, LOW);
    digitalWrite(in_2, LOW);
    analogWrite(en_12, 0);
    Serial.println("index exceeded expected range, stop the motor!");
  }
}

void test_drive_left_motor(int pwm_val, int max_pwm_allowed) {
  Serial.println("\ncalling test_drive_left_motor() with pwm_val of:" + String(pwm_val));
  if ((0 <= pwm_val) && (pwm_val <= max_pwm_allowed)) {
    digitalWrite(in_3, LOW);
    digitalWrite(in_4, HIGH);
    analogWrite(en_34, pwm_val);
    Serial.println("forward motion");
  } else if ((-max_pwm_allowed <= pwm_val) && (pwm_val <= 0)) {
    digitalWrite(in_3, HIGH);
    digitalWrite(in_4, LOW);
    analogWrite(en_34, -pwm_val);
    Serial.println("backward motion");
  } else {
    digitalWrite(in_3, LOW);
    digitalWrite(in_4, LOW);
    analogWrite(en_34, 0);
    Serial.println("index exceeded expected range, stop the motor!");
  }
}

void test_H_bridge_fully_on() {
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, HIGH);
  digitalWrite(en_34, HIGH);

  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  digitalWrite(en_12, HIGH);
}
