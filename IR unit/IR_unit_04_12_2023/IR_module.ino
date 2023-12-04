#define IR_RECEIVE_PIN 3
#define IR_LED 5
#define HALF_PERIOD_US 600
#define NUMBER_OF_PACKAGE_BITS 20
#define NUMBER_OF_PACKAGE_BITS_WO_CHECK 10

uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BITS];  //0:LSB, (NUMBER_OF_PACKAGE_BITS_WO_CHECK-1):MSB
uint8_t IR_module_buffer_status = 0;               //0: Floating, 1:Set, 2:Received, 3: Sent

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

void set_IR_module_buffer(uint8_t index, uint8_t value) {
  if (value > 1) {
    return;  //TODO: throw error
  }
  IR_module_buffer[index] = value;
  IR_module_buffer_status = 1;
}

uint8_t get_IR_module_buffer(uint8_t index) {
  return IR_module_buffer[index];
}

uint8_t get_IR_module_buffer_status() {
  return IR_module_buffer_status;
}

void receive_package_if_possible() {
  if (0 == digitalRead(IR_RECEIVE_PIN)) {
    delayMicroseconds(3 * HALF_PERIOD_US);
    uint8_t data_buffer[NUMBER_OF_PACKAGE_BITS];

    for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS; i++) {
      IR_module_buffer[i] = digitalRead(IR_RECEIVE_PIN);
      delayMicroseconds(2 * HALF_PERIOD_US);
    }

    print_buffer();
    IR_module_buffer_status = 2;
  }
}

void transmit_package() {
  send_IR_bit(0);  //start bit
  for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS; i++) {
    send_IR_bit(IR_module_buffer[i]);
  }
  IR_module_buffer_status = 3;
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

void print_buffer() {
  Serial.print(IR_module_buffer_status);
  Serial.print(": ");
  for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS; i++) {
    Serial.print(IR_module_buffer[i]);
  }
  Serial.println();
}