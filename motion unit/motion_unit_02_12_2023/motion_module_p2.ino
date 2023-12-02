

//Global variables


//=================================================

// void drive_right_motor(pwm_aimed) {
//   static pwm_current = 0;
//   static unsigned long updateTime = 0;
//   int step_size = 1;
//   int deltaT = 25;

//   int battery_voltage = 6;
//   float rated_pwm_max = test_set_rated_pwm_max(battery_voltage);

//   unsigned long currentTime = millis();                  // Get the current time
//   unsigned long elapsedTime = currentTime - updateTime;  // Calculate elapsed time

//     if (elapsedTime >= deltaT) {
//     int pwm_difference = pwm_aimed - pwm_current;
//     if (pwm_difference > 0) {  //increase PWM
//       pwm_current += min(step_size, pwm_difference);
//     } else if (pwm_difference < 0) {  //decrease PWM
//       pwm_current -= min(step_size, abs(pwm_difference));
//     }

//     // drive right motor
//       if ((0 <= pwm_current) && (pwm_current <= rated_pwm_max)) {
//     digitalWrite(in_1, HIGH);
//     digitalWrite(in_2, LOW);
//     analogWrite(en_12, pwm_current);
//     // forward motion
//   } else if ((-rated_pwm_max <= pwm_current) && (pwm_current < 0)) {
//     digitalWrite(in_1, LOW);
//     digitalWrite(in_2, HIGH);
//     analogWrite(en_12, -pwm_current);
//     // "backward motion
//   } else {
//     digitalWrite(in_1, LOW);
//     digitalWrite(in_2, LOW);
//     analogWrite(en_12, 0);
//     // "index exceeded expected range, stop the motor!
//   }

//   //update the updateTime
//     updateTime = elapsedTime;
//   }

// }


// void drive_left_motor(pwm_aimed) {
//   static pwm_current = 0;
//   static unsigned long updateTime = 0;
//   int step_size = 1;
//   int deltaT = 25;

//   int battery_voltage = 6;
//   float rated_pwm_max = test_set_rated_pwm_max(battery_voltage);

//   unsigned long currentTime = millis();                  // Get the current time
//   unsigned long elapsedTime = currentTime - updateTime;  // Calculate elapsed time

//     if (elapsedTime >= deltaT) {
//     int pwm_difference = pwm_aimed - pwm_current;
//     if (pwm_difference > 0) {  //increase PWM
//       pwm_current += min(step_size, pwm_difference);
//     } else if (pwm_difference < 0) {  //decrease PWM
//       pwm_current -= min(step_size, abs(pwm_difference));
//     }

//   //drive left motor
//   if ((0 <= pwm_current) && (pwm_current <= rated_pwm_max)) {
//     digitalWrite(in_3, LOW);
//     digitalWrite(in_4, HIGH);
//     analogWrite(en_34, pwm_current);
//     //forward motion
//   } else if ((-rated_pwm_max <= pwm_current) && (pwm_current <= 0)) {
//     digitalWrite(in_3, HIGH);
//     digitalWrite(in_4, LOW);
//     analogWrite(en_34, -pwm_current);
//     // backward motion
//   } else {
//     digitalWrite(in_3, LOW);
//     digitalWrite(in_4, LOW);
//     analogWrite(en_34, 0);
//     // index exceeded expected range, stop the motor!
//   }

//   //update tthe updateTime
//     updateTime = elapsedTime;
//   }

// }