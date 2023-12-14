//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 4

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define IGNORE_DURATION_IF_LESS_THAN_THIS_BURSTS 20
#define K_NUMBER_OF_BURSTS 40
#define TRIGGER_NUMBER_OF_BURSTS 30
#define ZERO_DURATION_IN_BURST_TERMS 30
#define ONE_DURATION_IN_BURST_TERMS 60
#define PREAMBLE_TRIGGER_DURATION_IN_BURST_TERMS 90
#define PREAMBLE_DURATION_IN_BURST_TERMS 90
#define LISTEN_TIMEOUT_IN_BURST_TERMS 200

#define BURST_HALF_PERIOD_US 13
unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * TRIGGER_NUMBER_OF_BURSTS;
unsigned long ZERO_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * ZERO_DURATION_IN_BURST_TERMS;
unsigned long ONE_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * ONE_DURATION_IN_BURST_TERMS;
unsigned long PREAMBLE_TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * PREAMBLE_TRIGGER_DURATION_IN_BURST_TERMS;
unsigned long PREAMBLE_HIGH_DURATION_US = (BURST_HALF_PERIOD_US * 2) * PREAMBLE_DURATION_IN_BURST_TERMS;
unsigned long LISTEN_TIMEOUT_DURATION_US = (BURST_HALF_PERIOD_US * 2) * LISTEN_TIMEOUT_IN_BURST_TERMS;
unsigned long IGNORE_DURATION_IF_LESS_THAN_THIS_DURATION_US = (BURST_HALF_PERIOD_US * 2) * IGNORE_DURATION_IF_LESS_THAN_THIS_BURSTS;

unsigned long K_DURATION_US = (BURST_HALF_PERIOD_US * 2) * K_NUMBER_OF_BURSTS;


uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];
uint8_t IR_module_buffer_status = 0;  //0: Floating, 1:Set, 2:Received, 3: Sent

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

//TRANSITTERS ========================================================
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

//RECEIVERS ========================================================

void receive_broadcast() {
  unsigned long receive_start_time = 0;
  unsigned long receive_duration = 0;

  uint8_t reading = digitalRead(IR_RECEIVE_PIN);
  if (reading == 0) {
    // A data broadcast is detected
    uint8_t last_state = 0;
    while (true) {
      reading = digitalRead(IR_RECEIVE_PIN);
      if (reading != last_state) {
        if (reading == 1) {
          // Rising edge detected (transition from 0 to 1)
          receive_start_time = micros();
        } else {
          // Falling edge detected (transition from 1 to 0)
          receive_duration = micros() - receive_start_time;

          // Compare with predefined values
          if (IGNORE_DURATION_IF_LESS_THAN_THIS_DURATION_US < receive_duration) {
            uint8_t received_bit = receive_duration / K_DURATION_US;
            if (received_bit == 3) {
              Serial.println();
            }
            Serial.print(received_bit);
          }
        }
      }
      if (micros() - receive_start_time > LISTEN_TIMEOUT_DURATION_US) {
        break;
      }
      last_state = reading;
    }
  }
}
