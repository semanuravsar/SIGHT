
#include "IR_module_header.h";
void setup() {
  Serial.begin(115200);

  initialize_IR_module();
}



uint8_t a = 0;
void loop() {

  if (Serial.available() > 0) {
    if (Serial.read() == 's') {
      Serial.println("Key 's' is pressed. Sending the data...");
      get_number_of_bytes();
      for (uint8_t i = 0; i < 255; i++) {
        set_buffer(0, i);
        set_buffer(1, i);
        uint16_t CRC_16 = generate_CRC_16_bit();
        uint8_t CRC_SIG = CRC_16 >> 8;
        uint8_t CRC_LST = CRC_16 % 256;
        set_buffer(3, CRC_LST);
        set_buffer(2, CRC_SIG);

        transmit_buffer();

        



        delay(10);
      }

      Serial.println("Transmitting process is completed.");
    }
  }
}