//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 1

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define K_NUMBER_OF_BURSTS 60
#define BURST_HALF_PERIOD_US 13

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
