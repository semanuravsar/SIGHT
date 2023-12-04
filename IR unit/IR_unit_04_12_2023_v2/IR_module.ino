//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3

#define NUMBER_OF_PACKAGE_BYTES 4

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define TRIGGER_NUMBER_OF_BURSTS 25
#define ZERO_DURATION_IN_BURST_TERMS 25
#define ONE_DURATION_IN_BURST_TERMS 40
#define PREAMBLE_TRIGGER_DURATION_IN_BURST_TERMS 100
#define PREAMBLE_DURATION_IN_BURST_TERMS 80

#define BURST_HALF_PERIOD_US 13
unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * TRIGGER_NUMBER_OF_BURSTS;
unsigned long ZERO_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * ZERO_DURATION_IN_BURST_TERMS;
unsigned long ONE_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * ONE_DURATION_IN_BURST_TERMS;
unsigned long PREAMBLE_TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * PREAMBLE_TRIGGER_DURATION_IN_BURST_TERMS;
unsigned long PREAMBLE_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * PREAMBLE_DURATION_IN_BURST_TERMS;


uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];
uint8_t IR_module_buffer_status = 0;  //0: Floating, 1:Set, 2:Received, 3: Sent

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}


unsigned long TRANSMISSION_START_TIME = 0;
void transmit_preamble() {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < PREAMBLE_TRIGGER_DURATION_US) {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(BURST_HALF_PERIOD_US);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(BURST_HALF_PERIOD_US);
  }
  delayMicroseconds(PREAMBLE_HIGH_DURATION_US);
}

void transmit_trigger() {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < TRIGGER_DURATION_US) {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(BURST_HALF_PERIOD_US);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(BURST_HALF_PERIOD_US);
  }
}

void transmit_zero() {
  transmit_trigger();
  delayMicroseconds(ZERO_HIGH_DURATION_US);
}

void transmit_one() {
  transmit_trigger();
  delayMicroseconds(ONE_HIGH_DURATION_US);
}
