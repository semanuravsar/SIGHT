    /*
  L298N DC MOTOR DRIVER MODULE
  modified on 25 Sep 2020
  by Saeed Olfat @ Electropeak
  Home
*/

void setup() {
  pinMode(8, OUTPUT); //IN2
  pinMode(9, OUTPUT); //IN1
  pinMode(10, OUTPUT); //Enable Pin
}
void loop() {
  delay(3000);

// 0 to 100% speed in forward mode
  for (int i=0;i<256;i++)
  {   digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      analogWrite(10, i);
      delay(50);      
      }

  delay(50);

  // 0 to 100% speed in backward mode
        for (int i=0;i<256;i++)
  {   digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      analogWrite(10, i);
      delay(50);      
      }

        delay(50);
}
