//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 1

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define K_NUMBER_OF_BURSTS 100
#define BURST_HALF_PERIOD_US 13
#define LISTEN_DURATION_MS 20

unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * K_NUMBER_OF_BURSTS;

uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

//TRANSITTERS ========================================================
unsigned long TRANSMISSION_START_TIME = 0;

void transmit_zero() {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < TRIGGER_DURATION_US) {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(BURST_HALF_PERIOD_US);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(BURST_HALF_PERIOD_US);
  }
}


void transmit_one() {
  TRANSMISSION_START_TIME = micros();
  digitalWrite(IR_LED, LOW);
  delayMicroseconds(TRIGGER_DURATION_US);
}


void listen_IR() {
  unsigned long listen_start_time = millis();
  uint8_t counter = 0;
  while (millis() - listen_start_time < LISTEN_DURATION_MS) {
    if (digitalRead(IR_RECEIVE_PIN) == 0) {
      delayMicroseconds(BURST_HALF_PERIOD_US);
      counter = counter + 1;

      if (counter > 5) {
        Serial.println(counter);
        break;
      }
    }
  }

  //check if transmission is detected
  if (counter>5) {
    delayMicroseconds(TRIGGER_DURATION_US * 1.5);
    for (uint8_t i = 0; i < 16; i++) {
      Serial.print(digitalRead(IR_RECEIVE_PIN));
      delayMicroseconds(TRIGGER_DURATION_US);
    }
    Serial.println();

    //start sampling
  }
}
