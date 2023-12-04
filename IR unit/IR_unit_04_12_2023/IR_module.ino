#define IR_RECEIVE_PIN 3
#define IR_LED 5
#define HALF_PERIOD_US 700
#define NUMBER_OF_PACKAGE_BITS 16
#define NUMBER_OF_PACKAGE_BITS_WO_CHECK 8

void send_IR(uint8_t);

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

void receive_package_if_possible() {
  if (0 == digitalRead(IR_RECEIVE_PIN)) {
    delayMicroseconds(3 * HALF_PERIOD_US);
    uint8_t data_buffer[NUMBER_OF_PACKAGE_BITS];  //0:LSB, 7:MSB

    for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS; i++) {
      data_buffer[i] = digitalRead(IR_RECEIVE_PIN);
      delayMicroseconds(2 * HALF_PERIOD_US);
    }

    uint8_t received_val = 0;
    uint8_t inverted_received_val = 0;
    for (int i = (NUMBER_OF_PACKAGE_BITS_WO_CHECK-1); i >= 0; i--) {
      received_val = (received_val << 1) + data_buffer[i];
      inverted_received_val = (inverted_received_val << 1) + data_buffer[i + NUMBER_OF_PACKAGE_BITS_WO_CHECK];
    }

    if (inverted_received_val + received_val == 255) {
      Serial.println("Received val:" + String(received_val));
    }
  }
}

void send_IR_bit(uint8_t single_bit) {
  if (single_bit == 0) {
    unsigned long start_time = micros();
    while (micros() - start_time < HALF_PERIOD_US) {
      digitalWrite(IR_LED, LOW);
      delayMicroseconds(10);
      digitalWrite(IR_LED, HIGH);
      delayMicroseconds(10);
    }
    delayMicroseconds(HALF_PERIOD_US);
  } else {
    delayMicroseconds(2 * HALF_PERIOD_US);
  }
}

void transmit_byte_package(uint8_t byte_data) {
  uint8_t inverted_byte_data = ~byte_data;
  send_IR_bit(0);  //start bit
  for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS_WO_CHECK; i++) {
    send_IR_bit(byte_data & 1);
    byte_data = byte_data >> 1;
  }
  for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS_WO_CHECK; i++) {
    send_IR_bit(inverted_byte_data & 1);
    inverted_byte_data = inverted_byte_data >> 1;
  }
}
