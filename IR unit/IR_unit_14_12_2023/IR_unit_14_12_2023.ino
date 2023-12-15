
#include "IR_module_header.h";
void setup() {
  Serial.begin(115200);

  initialize_IR_module();
}

void loop() {
  sender_trial();
  //listener_trial();
}


void listener_trial() {
  listen_IR();
}
void sender_trial() {

  if (Serial.available() > 0) {
    if (Serial.read() == 's') {
      Serial.println("Key 's' is pressed. Sending the data...");

      for (uint8_t i = 0; i < 1; i++) {
        set_buffer(0, 15);
        set_buffer(1, 20);
        uint16_t CRC_16 = generate_CRC_16_bit();
        uint8_t CRC_SIG = CRC_16 >> 8;
        uint8_t CRC_LST = CRC_16 % 256;
        set_buffer(2, CRC_SIG);
        set_buffer(3, CRC_LST);
        Serial.println("15 20 " + String(CRC_SIG) + " " + String(CRC_LST));
        transmit_buffer();
        delay(100);
      }

      Serial.println("Transmitting process is completed.");
    }
  }
}
