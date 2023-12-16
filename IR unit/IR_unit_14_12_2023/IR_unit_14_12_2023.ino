
#include "IR_module_header.h";
void setup() {
  Serial.begin(115200);

  initialize_IR_module();
}

void loop() {
  communication_test();
}


void communication_test() {
  //To be used in test 1 and test 2
  static unsigned long succesful_package_counter = 0;
  static unsigned long corrupted_package_counter = 0;


  uint8_t listening_result = listen_IR();
  if (listening_result == 1) {
    succesful_package_counter = succesful_package_counter + 1;
  } else if (listening_result == 2) {
    corrupted_package_counter = corrupted_package_counter + 1;
  }

  if (Serial.available() > 0) {
    int incoming_byte = Serial.read();
    get_number_of_bytes()

      if (incoming_byte == 'S') {
      Serial.println("Sending the data...");
      for (uint16_t i = 0; i < 256; i++) {
        set_buffer(0, i);
        set_buffer(1, i);
        uint16_t CRC_16 = generate_CRC_16_bit();
        uint8_t CRC_SIG = CRC_16 >> 8;
        uint8_t CRC_LST = CRC_16 % 256;
        set_buffer(NUMBER_OF_PACKAGE_BYTES - 2, CRC_SIG);
        set_buffer(NUMBER_OF_PACKAGE_BYTES - 1, CRC_LST);
        transmit_buffer();
        Serial.println(String(i) + " / " + String(255));
        delay(20);
      }
      Serial.println("Transmitting process is completed.");
    }
    else if (incoming_byte == 'B') {
      Serial.println("\nSending random data...");
      uint8_t randNumber_1 = random(256);
      uint8_t randNumber_2 = random(256);
      set_buffer(0, randNumber_1);
      set_buffer(1, randNumber_2);

      uint16_t CRC_16 = generate_CRC_16_bit();
      uint8_t CRC_SIG = CRC_16 >> 8;
      uint8_t CRC_LST = CRC_16 % 256;
      set_buffer(NUMBER_OF_PACKAGE_BYTES - 2, CRC_SIG);
      set_buffer(NUMBER_OF_PACKAGE_BYTES - 1, CRC_LST);
      transmit_buffer();

      Serial.println(String(randNumber_1) + " " + String(randNumber_2) + " " + String(CRC_SIG) + " " + String(CRC_LST));
      delay(20);
    }
    else if (incoming_byte == 'P') {
      Serial.println("\n Package Size Changed");
      while (Serial.available() == 0) {}
      int package_number = Serial.read();
      if ('3' < package_number && package_number < '7') {

        NUMBER_OF_PACKAGE_BYTES = package_number - '0';
        for (uint16_t i = 0; i < package_number; i++) {
          set_buffer(i, randNumber_1);
        }
      }
    }
    else if (incoming_byte == 'L') {
      Serial.println("\nTotal packages received:" + String(succesful_package_counter + corrupted_package_counter));
      Serial.println("Succesfull packages:" + String(succesful_package_counter));
      Serial.println("Corrupted packages:" + String(corrupted_package_counter));
      Serial.println("Succes ratio is left as an exercise = " + String(succesful_package_counter) + " / " + String(succesful_package_counter + corrupted_package_counter));
    }
    else if (incoming_byte == 'R') {
      Serial.println("\nCounters are reseted");
      succesful_package_counter = 0;
      corrupted_package_counter = 0;
    }
  }
}
