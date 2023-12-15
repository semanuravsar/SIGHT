//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 4

//delayMicroseconds() very accurately in the range 3 microseconds and up to 16383. Be careful if you are not in this range
#define K_NUMBER_OF_BURSTS 60
#define BURST_HALF_PERIOD_US 13
#define LISTEN_DURATION_MS 20
#define TRANSMIT_SLEEP_TIME 20
unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * K_NUMBER_OF_BURSTS;

uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];


uint16_t get_number_of_bytes(){
  return NUMBER_OF_PACKAGE_BYTES;
}



void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

// =============================================================00000




void set_buffer(uint16_t byte_index, uint8_t byte_value) {
  uint8_t reversed_byte = 0;

  for (int i = 0; i < 8; i++) {
    if (byte_value & (1 << i)) {
      reversed_byte |= (1 << (7 - i));
    }
  }

  IR_module_buffer[byte_index] = reversed_byte;
}

void transmit_buffer() {


  transmit_zero();  //start bit

  for (uint16_t byte_index = 0; byte_index < NUMBER_OF_PACKAGE_BYTES; byte_index++) {
    for (uint8_t package_bit = 0; package_bit < 8; package_bit++) {
      uint8_t send_bit = (IR_module_buffer[byte_index] & 1);
      if (send_bit == 0) {
        transmit_zero();
      } else {
        transmit_one();
      }
      IR_module_buffer[byte_index] = IR_module_buffer[byte_index] >> 1;
    }
  }

  
}
//TRANSITTERS ========================================================
unsigned long TRANSMISSION_START_TIME = 0;

void transmit_zero() {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < (TRIGGER_DURATION_US)) {
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
        break;
      }
    }
  }

  //check if transmission is detected
  if (counter > 5) {
    delayMicroseconds(TRIGGER_DURATION_US * 1.5);
    unsigned long listen_starts = micros();
    for (uint8_t i = 0; i < 32; i++) {
      uint8_t byte_no = i / 8;

      IR_module_buffer[byte_no] = (IR_module_buffer[byte_no] << 1) + digitalRead(IR_RECEIVE_PIN);

      //Serial.print(digitalRead(IR_RECEIVE_PIN));
      while (micros() < listen_starts + (i + 1) * TRIGGER_DURATION_US) {
        continue;
      }
    }

    uint16_t CRC_16 = generate_CRC_16_bit();
    uint8_t CRC_SIG = CRC_16 >> 8;
    uint8_t CRC_LST = CRC_16 % 256;
    if (IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 1] == CRC_LST && IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 2] == CRC_SIG) {
      for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BYTES; i++) {
        Serial.print(IR_module_buffer[i]);
        Serial.print(' ');
      }
      Serial.println();
    } else {
      Serial.println("Package corrupted");
    }

    delay(10);
    //start sampling
  }
}


//MAGICAL CRC_16 MODBUS code.
uint16_t generate_CRC_16_bit() {
  uint16_t remainder = CRC_16_bit_for_1BYTE(IR_module_buffer[0], 65535);
  for (uint8_t i = 1; i < NUMBER_OF_PACKAGE_BYTES - 2; i++) {
    remainder = CRC_16_bit_for_1BYTE(IR_module_buffer[i], remainder);
  }
  return remainder;
}
uint16_t CRC_16_bit_for_1BYTE(uint16_t data, uint16_t last_data) {
  //if this is first data (i.e LAST_DATA==null), LAST_DATA= 65535 = FFFF
  uint16_t key = 40961;     //1010 0000 0000 0001
  data = data ^ last_data;  //XOR
  for (int i = 0; i < 8; i++) {
    boolean should_XOR = false;
    if (data % 2 == 1) should_XOR = true;
    data = data >> 1;
    if (should_XOR) data = data ^ key;
  }
  return data;
}
