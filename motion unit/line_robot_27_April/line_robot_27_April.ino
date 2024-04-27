#include <QTRSensors.h>

// D2 sağ, D7 sol
#define Kp 0.4 // Kendi deneyimlerinizle bulmanız gerekli küçük bir değer ile başlayıp, büyütebilirsiniz en kararlı Kp değerini bulana kadar.. 0.4
#define Kd 2 // Bunu da küçük bir değerden başlatın ve deneyimleyerek büyütün. ( Not: Kp < Kd) 2.0
#define rightMaxSpeed 196
#define leftMaxSpeed 196
#define rightBaseSpeed 127 // robot için kp ve kd değerlerini tutturduysanız şayet motorların dönmesi gereken hız budur
#define leftBaseSpeed 127 // yukarıdaki değer ile aynıdır


#define rightMotor1 13
#define rightMotor2 12
#define rightMotorPWM 11
#define leftMotor1 8
#define leftMotor2 7
#define leftMotorPWM 10


/*  digitalWrite(rightMotor1,HIGH);
    digitalWrite(rightMotor2, LOW); sağ motor ileri

    digitalWrite(leftMotor1,LOW);
    digitalWrite(leftMotor2, HIGH); sol motor ileri
*/

QTRSensors qtr;

const uint8_t SensorCount = 6;
unsigned int sensorValues[SensorCount];
int rightMotorSpeed;
int leftMotorSpeed;



void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  uint8_t analog_pins[6] = {A0,A1,A2,A3,A4,A5};   //A5 = D7 ... A0 = D2, !!! D8 & D1 are not used
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode


  // int i;
  // for (int i = 0; i < 100; i++) { // tercihe bağlı alandır ya robotunuzu hatta yürüterek kalibre edin veya bunu otomatik yapın.
  //   //otomatik kalibrasyon için burasının yorumunu kaldırın
  //   if ( i  < 25 || i >= 75 ) {// sensörleri, karşılaşılabilecek en aydınlık ve en karanlık okumalara maruz bırakmak için sola ve sağa çevirin.
  //     turn_right();
  //   } //bu fonksiyonları yazacaz
  //   else {
  //     turn_left();
  //   }
  //   qtr.calibrate();
  //   delay(20);
  // }
  // wait(); //motorları durdur
  // digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // delay(1000); // Ana döngüye girmeden önce botu konumlandırmak için 1 saniye bekleyin
}

void loop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 7000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);
  Serial.println(position);

  

    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorPWM, rightMotorSpeed);

    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    analogWrite(leftMotorPWM, leftMotorSpeed);
  
}
void wait() {
      digitalWrite(rightMotor1,LOW);
    digitalWrite(rightMotor2, LOW);
   

    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    
}
void turn_left() {
      digitalWrite(rightMotor1,HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite (rightMotorPWM,rightBaseSpeed); 

    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite (leftMotorPWM,leftBaseSpeed);
}
void turn_right() {
      digitalWrite(rightMotor1,LOW);
    digitalWrite(rightMotor2, HIGH);
    analogWrite (rightMotorPWM,rightBaseSpeed);   

    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite (leftMotorPWM,leftBaseSpeed);    
}