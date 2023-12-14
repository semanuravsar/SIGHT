//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 4

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define K_NUMBER_OF_BURSTS 60
#define BURST_HALF_PERIOD_US 13
#define LISTEN_DURATION_MS 20

unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * K_NUMBER_OF_BURSTS;

uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

// =============================================================00000
void set_buffer(uint16_t byte_index, uint8_t byte_value) {
  IR_module_buffer[byte_index] = byte_value;
}

void transmit_buffer() {


  transmit_zero(0);  //start bit
  unsigned long spit_starts = micros();
  int duration_offset = 0;
  for (uint16_t byte_index = 0; byte_index < NUMBER_OF_PACKAGE_BYTES; byte_index++) {
    for (uint8_t package_bit = 0; package_bit < 8; package_bit++) {
      uint8_t send_bit = (IR_module_buffer[byte_index] & 1);
      if (send_bit == 0) {
        transmit_zero(duration_offset);
      } else {
        transmit_one(duration_offset);
      }
      IR_module_buffer[byte_index] = IR_module_buffer[byte_index] >> 1;
    }
  }
}
//TRANSITTERS ========================================================
unsigned long TRANSMISSION_START_TIME = 0;

void transmit_zero(int duration_offset) {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < (TRIGGER_DURATION_US + duration_offset)) {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(BURST_HALF_PERIOD_US);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(BURST_HALF_PERIOD_US);
  }
}

void transmit_one(int duration_offset) {
  TRANSMISSION_START_TIME = micros();
  digitalWrite(IR_LED, LOW);
  delayMicroseconds(TRIGGER_DURATION_US + duration_offset);
}

void listen_IR() {
  unsigned long listen_start_time = millis();
  uint8_t counter = 0;
  while (millis() - listen_start_time < LISTEN_DURATION_MS) {
    if (digitalRead(IR_RECEIVE_PIN) == 0) {
      delayMicroseconds(BURST_HALF_PERIOD_US);
      counter = counter + 1;

      if (counter > 5) {
        break;
      }
    }
  }

  //check if transmission is detected
  if (counter > 5) {
    delayMicroseconds(TRIGGER_DURATION_US * 1.5);
    unsigned long listen_starts = micros();
    for (uint8_t i = 0; i < 48; i++) {
      Serial.print(digitalRead(IR_RECEIVE_PIN));
      while (micros() < listen_starts + (i + 1) * TRIGGER_DURATION_US) {
        continue;
      }
    }
    Serial.println();

    //start sampling
  }
}
