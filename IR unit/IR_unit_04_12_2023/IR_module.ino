#define IR_RECEIVE_PIN 3
#define IR_LED 5
#define HALF_PERIOD_US 600
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
    for (int i = 7; i >= 0; i--) {
      received_val = (received_val << 1) + data_buffer[i];
      inverted_received_val = (inverted_received_val << 1) + data_buffer[i + NUMBER_OF_PACKAGE_BITS_WO_CHECK];
    }

    Serial.println("Received val:" + String(received_val));
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
  for (uint8_t i = 0; i < 8; i++) {
    send_IR_bit(byte_data & 1);
    Serial.print(byte_data & 1);
    byte_data = byte_data >> 1;
  }
  Serial.print(" ");
  for (uint8_t i = 0; i < 8; i++) {
    send_IR_bit(inverted_byte_data & 1);
    Serial.print(inverted_byte_data & 1);
    inverted_byte_data = inverted_byte_data >> 1;
  }
  Serial.println();
}



// void send_byte(uint8_t byte_value){
//   send_IR_bit(0);//start bit
//   for (int i = 7; i >= 0; i--) {
//     send_IR_bit((byte_value >> i) & 1);
//   }

//   uint8_t inverter_byte_value = ~byte_value;
//   for (int i = 7; i >= 0; i--) {
//     send_IR_bit((inverter_byte_value >> i) & 1);
//   }
// }
// void send_IR_bit(uint8_t val_to_send){

//       if(val_to_send == 0){
//         unsigned long start_time = micros();
//         while(micros()-start_time <HALF_PERIOD_US){
//           digitalWrite(IR_LED, LOW);
//           delayMicroseconds(10);
//           digitalWrite(IR_LED, HIGH);
//           delayMicroseconds(10);
//         }
//         delayMicroseconds(HALF_PERIOD_US);
//       }else{
//         delayMicroseconds(2*HALF_PERIOD_US);
//       }



// }
// void count_IR() {
//   if (0 == digitalRead(IR_RECEIVE_PIN)) {
//     delayMicroseconds(3 * HALF_PERIOD_US);
//     uint8_t data_buffer[NUMBER_OF_PACKAGE_BITS];  //0:MSB, 7:LSB

//     for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BITS; i++) {
//       data_buffer[i] = digitalRead(IR_RECEIVE_PIN);
//       delayMicroseconds(2 * HALF_PERIOD_US);
//     }

//     uint8_t received_val = data_buffer[0] * 128 + data_buffer[1] * 64 + data_buffer[2] * 32 + data_buffer[3] * 16 + data_buffer[4] * 8 + data_buffer[5] * 4 + data_buffer[6] * 2 + data_buffer[7];
//     uint8_t inverted_received_val = data_buffer[8] * 128 + data_buffer[9] * 64 + data_buffer[10] * 32 + data_buffer[11] * 16 + data_buffer[12] * 8 + data_buffer[13] * 4 + data_buffer[14] * 2 + data_buffer[15];

//     Serial.println(received_val);
//     if (received_val + inverted_received_val == 255) {
//       Serial.println(received_val);
//     } else {
//       Serial.println("Corrupted Data");
//     }
//   }
// }

// void send

// #define IR_LED 5
// #define HALF_PERIOD_US 600
// #define NUMBER_OF_PACKAGE_BITS 16

// void send_IR(uint8_t);

// void setup(){
//   pinMode(IR_LED, OUTPUT);
//   Serial.begin(115200);
// }

// void loop(){
//   // for(int i = 1; i<10; i++){
//   //   send_IR(i);
//   // }

//   for(int i =0; i<255; i++){
//   send_byte(i);
//   delay(50);
//   }

// }

// void send_byte(uint8_t byte_value){
//   send_IR_bit(0);//start bit
//   for (int i = 7; i >= 0; i--) {
//     send_IR_bit((byte_value >> i) & 1);
//   }

//   uint8_t inverter_byte_value = ~byte_value;
//   for (int i = 7; i >= 0; i--) {
//     send_IR_bit((inverter_byte_value >> i) & 1);
//   }
// }

// void send_IR_bit(uint8_t val_to_send){
//       if(val_to_send == 0){
//         unsigned long start_time = micros();
//         while(micros()-start_time <HALF_PERIOD_US){
//           digitalWrite(IR_LED, LOW);
//           delayMicroseconds(10);
//           digitalWrite(IR_LED, HIGH);
//           delayMicroseconds(10);
//         }
//         delayMicroseconds(HALF_PERIOD_US);
//       }else{
//         delayMicroseconds(2*HALF_PERIOD_US);
//       }
// }