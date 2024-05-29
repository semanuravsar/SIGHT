//Refer to: https://www.ersinelektronik.com/class/INNOVAEditor/assets/Datasheets/TSOP1136.pdf

uint8_t NUMBER_OF_PACKAGE_BYTES = 4;  //cannot be smaller than 3.
unsigned long TRIGGER_DURATION_US = (BURST_HALF_PERIOD_US * 2) * K_NUMBER_OF_BURSTS;
int loop_iteration_wo_ping_message = 0;
uint8_t ping_time_out = 0;
uint8_t ping_timer = millis();

uint8_t IR_module_buffer[32];

void initialize_IR_module() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(IR_LED, OUTPUT);
}

uint8_t get_buffer(uint16_t byte_index) {
  return IR_module_buffer[byte_index];
}

// =============================================================
uint16_t get_number_of_package_bytes() {
  return NUMBER_OF_PACKAGE_BYTES;
}
void set_number_of_package_bytes(uint16_t new_package_size) {
  NUMBER_OF_PACKAGE_BYTES = new_package_size;
}

void set_buffer(uint16_t byte_index, uint8_t byte_value) {
  IR_module_buffer[byte_index] = byte_value;
}

void reverse_buffer() {
  for (int j = 0; j < NUMBER_OF_PACKAGE_BYTES; j++) {
    uint8_t reversed_byte = 0;
    for (int i = 0; i < 8; i++) {
      if (IR_module_buffer[j] & (1 << i)) {
        reversed_byte |= (1 << (7 - i));
      }
    }
    IR_module_buffer[j] = reversed_byte;
  }
}

void transmit_buffer() {

  reverse_buffer();
  //=================

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

  //=================
  reverse_buffer();
}
//TRANSITTERS ========================================================
unsigned long TRANSMISSION_START_TIME = 0;

void transmit_zero() {
  TRANSMISSION_START_TIME = micros();
  while (micros() - TRANSMISSION_START_TIME < (TRIGGER_DURATION_US-30)) {
    // 32 us
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(BURST_HALF_PERIOD_US);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(BURST_HALF_PERIOD_US);
  }
}

void transmit_one() {
  TRANSMISSION_START_TIME = micros();
  // 12 us
  digitalWrite(IR_LED, LOW);
  delayMicroseconds(TRIGGER_DURATION_US-10);
}

void shift_reg_insert_zero() {
  PORTB = PORTB & B11111110; //define SHIFT_REG_INPUT 8 (portb-0)->LOW
  //digitalWrite(SHIFT_REG_INPUT, LOW);
  delayMicroseconds(1);

  PORTB = PORTB & B11111101; //#define SHIFT_REG_CLK_PIN 9 (portb-1)->LOW
  //digitalWrite(SHIFT_REG_CLK_PIN, LOW);
  delayMicroseconds(1);

  PORTB = PORTB | B00000010; //#define SHIFT_REG_CLK_PIN 9 (portb-1)->HIGH
  //digitalWrite(SHIFT_REG_CLK_PIN, HIGH);
  delayMicroseconds(1);
}

void shift_reg_insert_one() {
  PORTB = PORTB | B00000001; //define SHIFT_REG_INPUT 8 (portb-0)->HIGH
  //digitalWrite(SHIFT_REG_INPUT, LOW);
  delayMicroseconds(1);

  PORTB = PORTB & B11111101; //#define SHIFT_REG_CLK_PIN 9 (portb-1)->LOW
  //digitalWrite(SHIFT_REG_CLK_PIN, LOW);
  delayMicroseconds(1);

  PORTB = PORTB | B00000010; //#define SHIFT_REG_CLK_PIN 9 (portb-1)->HIGH
  //digitalWrite(SHIFT_REG_CLK_PIN, HIGH);
  delayMicroseconds(1);
}

void zero_shift_register() {
  for (uint8_t i = 0; i < 8; i++) {
    shift_reg_insert_zero();
  }
}

uint8_t listen_IR() {
  digitalWrite(IR_LED, LOW);

  unsigned long listen_start_time = millis();
  uint8_t is_received = 0;

  uint8_t shift_index = 0;
  zero_shift_register();
  while (millis() - listen_start_time < LISTEN_DURATION_MS) {
    shift_reg_insert_one();
    for (shift_index = 0; shift_index < 8; shift_index++) {
      if (digitalRead(IR_RECEIVE_PIN) == 1) {
        is_received = 1;
        break;
      }
      shift_reg_insert_zero();
    }
    if (is_received)break;
  }

  //check if transmission is detected
  if (is_received == 1) {
    delayMicroseconds(TRIGGER_DURATION_US * 1.5);
    unsigned long listen_starts = micros();
    for (uint8_t i = 0; i < (NUMBER_OF_PACKAGE_BYTES * 8); i++) {
      uint8_t byte_no = i / 8;
      uint8_t read_bit = digitalRead(IR_RECEIVE_PIN);
      if (read_bit == 1) {
        read_bit = 0;
      } else {
        read_bit = 1;
      }
      IR_module_buffer[byte_no] = (IR_module_buffer[byte_no] << 1) + read_bit;
      while (micros() < listen_starts + (i + 1) * TRIGGER_DURATION_US) {
        continue;
      }
    }

    uint16_t CRC_16 = generate_CRC_16_bit();
    uint8_t CRC_SIG = CRC_16 >> 8;
    uint8_t CRC_LST = CRC_16 % 256;
    if (IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 1] == CRC_LST && IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 2] == CRC_SIG) {
      // Serial.print(String(shift_index)+" -> ");
      for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BYTES; i++) {
        // Serial.print(IR_module_buffer[i]);
        // Serial.print(' ');
      }
      // Serial.println();
      return 1;  //Package is valid. return 1
    } else {
      // Serial.println("Package corrupted");
      return 2;  // CRC check is failed. return 2
    }
  }

  return 0;  // No signal is detected. return 0
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


void send_target_info_old(int currentPosition_x, int currentPosition_y, int targetPosition_x, int targetPosition_y) {
  
  uint8_t first_byte = currentPosition_x; 
  uint8_t second_byte = currentPosition_y; 
  uint8_t third_byte = targetPosition_x;
  uint8_t fourth_byte = targetPosition_y;

  set_buffer(0, first_byte);
  set_buffer(1, second_byte);
  set_buffer(2, third_byte);
  set_buffer(3, fourth_byte);
  // The 5th and 6th bits are just set to 0
  set_buffer(4, 0);
  set_buffer(5, 0);

  uint16_t CRC_16 = generate_CRC_16_bit();
  uint8_t CRC_SIG = CRC_16 >> 8;
  uint8_t CRC_LST = CRC_16 % 256;
  // Serial.println(String(CRC_SIG) + " " + String(CRC_LST));
  set_buffer(6, CRC_SIG);
  set_buffer(7, CRC_LST);

  transmit_buffer();
  delay(20); 
  
}


//ping_test() yerine
void send_ping_message(Position MU2_NextPosition, Position targetPosition, int ID, uint8_t MU2_state){  
  
  uint8_t nextPosition_x = MU2_NextPosition.x;
  uint8_t nextPosition_y = MU2_NextPosition.y;
  uint8_t targetPosition_x = targetPosition.x;
  uint8_t targetPosition_y = targetPosition.y;

  // uint8_t nextPosition_x = 2;
  // uint8_t nextPosition_y = 2;
  // uint8_t targetPosition_x = 1;
  // uint8_t targetPosition_y = 3;

  uint8_t first_byte = nextPosition_x*16 + nextPosition_y; 
  uint8_t second_byte = MU2_state*16 + ID; 
  uint8_t third_byte = targetPosition_x*16 + targetPosition_y;

  // uint8_t first_byte = nextPosition_x*16 + nextPosition_y; 
  // uint8_t second_byte = MU1_state*16 + 2; 
  // uint8_t third_byte = targetPosition_x*16 + targetPosition_y;

  set_buffer(0, first_byte);
  set_buffer(1, second_byte);
  set_buffer(2, third_byte);

  uint16_t CRC_16 = generate_CRC_16_bit();
  uint8_t CRC_SIG = CRC_16 >> 8;
  uint8_t CRC_LST = CRC_16 % 256;
  set_buffer(3, CRC_SIG);
  // set_buffer(4, CRC_LST);
    
  transmit_buffer(); 
  
}

// send_ping_ack yerine
void send_ack_message(Position MU2_NextPosition, Position targetPosition, int ID, uint8_t MU2_state) {
  
  uint8_t nextPosition_x = MU2_NextPosition.x;
  uint8_t nextPosition_y = MU2_NextPosition.y;
  uint8_t targetPosition_x = targetPosition.x;
  uint8_t targetPosition_y = targetPosition.y;

  uint8_t first_byte = nextPosition_x*16 + nextPosition_y; 
  uint8_t second_byte = (MU2_state+9)*16 + ID; 
  uint8_t third_byte = targetPosition_x*16 + targetPosition_y;

  set_buffer(0, first_byte);
  set_buffer(1, second_byte);
  set_buffer(2, third_byte);

  uint16_t CRC_16 = generate_CRC_16_bit();
  uint8_t CRC_SIG = CRC_16 >> 8;
  uint8_t CRC_LST = CRC_16 % 256;
  set_buffer(3, CRC_SIG);  
  transmit_buffer();
  
}

void final_communication(Position& MU1_NextPosition, Position& MU2_NextPosition, Position& MU3_NextPosition, Position& targetPosition, int ID, uint8_t& MU1_state, uint8_t& MU2_state, uint8_t& MU3_state, int& buLink, uint8_t& active_s){

 
  // We do NOT send a ping if we haven't listened to a ping for 3 loops
  if(loop_iteration_wo_ping_message<3 || ping_time_out){

    active_s = active_s + 1;
    if(active_s == 8){active_s =0;}
    set_active_s(active_s);
    send_ping_message(MU2_NextPosition, targetPosition, ID, MU2_state);

    // We record the last time we sent a ping
    ping_timer = millis();

  }

  if(millis()-ping_timer < 100){
    ping_time_out = 0;
  }
  // If we haven't been sending pings for 500 ms
  else{ping_time_out = 1;}


  if(active_s == 4){
    loop_iteration_wo_ping_message++;
  }

  listener(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition,MU1_state,MU2_state, MU3_state, buLink, loop_iteration_wo_ping_message);

}

void listener(Position& MU1_NextPosition, Position& MU2_NextPosition, Position& MU3_NextPosition, Position& targetPosition,uint8_t& MU1_state,uint8_t& MU2_state, uint8_t& MU3_state, int& buLink, uint8_t loop_iteration_wo_ping_message){
      
  // Serial.println("Listening rn...");
  // We listen for 20 ms at a time
  uint8_t listening_result = listen_IR_Erdem(); // 0:no package, 1:successful package, 2:corrupted package
  if(listening_result == 2){listening_result = listen_IR_Erdem();}

  // Serial.println("Listening result: ");
  // Serial.println(listening_result);

  
  if (listening_result == 1){ // i.e., the message received is succesful
    
    uint8_t  received_state = 0;

    uint8_t first_byte = get_buffer(0);
    uint8_t second_byte = get_buffer(1);
    uint8_t third_byte = get_buffer(2);

    
    uint8_t  received_x = first_byte >> 4;
    uint8_t  received_y = first_byte % 16;

    uint8_t  intention = second_byte >> 4;
    uint8_t  received_ID = second_byte % 16;

    uint8_t  target_x = third_byte >> 4;
    uint8_t  target_y = third_byte % 16;


    
    // Determining the message intention
    if(intention>9){
      received_state = intention-9;
      intention = 2;
    }
    else{
      received_state = intention;
      intention = 1;
    }

    // Serial.println("x: " + String(received_x) + " y: " + String(received_y) + " state: " + String(received_state) + " received_ID: " + String(received_ID) + " target x: " + String(target_x)+ " target y: " + String(target_y)+ "\n");



    // We update the info we have on the units depending on the info we received.
    if(received_ID == 1){
      MU1_NextPosition.x = received_x;
      MU1_NextPosition.y = received_y;
      MU1_state = received_state;

      if(target_x < 10 && target_x > 0){

        targetPosition.x = target_x;
        targetPosition.y = target_y;
      }
    }  
    else if(received_ID == 3){
      MU3_NextPosition.x = received_x;
      MU3_NextPosition.y = received_y;
      MU3_state = received_state;
      if(target_x < 10 && target_x > 0){
        targetPosition.x = target_x;
        targetPosition.y = target_y;
      }
    }
    else if(received_ID == 4){
      
      buLink = 1;
      if(target_x < 10 && target_x > 0){
        targetPosition.x = target_x;
        targetPosition.y = target_y;
      }

    }   

    if(intention == 1){

      loop_iteration_wo_ping_message = 0;
      // Serial.println("Ping PACKAGE is received, sending ping ack");
      send_ack_message(MU2_NextPosition, targetPosition, ID, MU2_state);  
      send_ack_message(MU2_NextPosition, targetPosition, ID, MU2_state); 
                       
      return;
    }

    else if(intention == 2){ // Acknowledgement is received
      // Serial.println("Ping ACKNOWLEDGEMENT is received.");
      return;
    }
  }
  
}

void set_active_s(uint8_t pick_this_s) {
  
  int led_to_i_mapping[9] = { 4, 5, 6, 7, 0, 1, 2, 3, 999 };  // do not alter, physically linked
  digitalWrite(SHIFT_REG_INPUT, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    if (i == led_to_i_mapping[pick_this_s]) {
      digitalWrite(SHIFT_REG_INPUT, HIGH);
    } else {
      digitalWrite(SHIFT_REG_INPUT, LOW);
    }
    delayMicroseconds(5);

    digitalWrite(SHIFT_REG_CLK_PIN, LOW);
    delayMicroseconds(25);
    digitalWrite(SHIFT_REG_CLK_PIN, HIGH);
    delayMicroseconds(25);
  }
}

uint8_t listen_IR_Erdem() {
  digitalWrite(IR_LED, LOW);
  delayMicroseconds(500);


  unsigned long listen_start_time = millis();
  uint8_t is_received = 0;

  uint8_t shift_index = 0;
  zero_shift_register();
  while (millis() - listen_start_time < LISTEN_DURATION_MS) {
    shift_reg_insert_one();
    for (shift_index = 0; shift_index < 8; shift_index++) {
      
      if (digitalRead(IR_RECEIVE_PIN) == 1) {
        is_received = 1;
        break;
      }
      shift_reg_insert_zero();
    }
    if (is_received)break;
  }

  //check if transmission is detected
  if (is_received == 1) {
    delayMicroseconds(TRIGGER_DURATION_US * 1.5);
    unsigned long listen_starts = micros();
    for (uint8_t i = 0; i < (NUMBER_OF_PACKAGE_BYTES * 8); i++) {
      uint8_t byte_no = i / 8;
      uint8_t read_bit = digitalRead(IR_RECEIVE_PIN);
      if (read_bit == 1) {
        read_bit = 0;
      } else {
        read_bit = 1;
      }
      IR_module_buffer[byte_no] = (IR_module_buffer[byte_no] << 1) + read_bit;
      while (micros() < listen_starts + (i + 1) * TRIGGER_DURATION_US) {
        continue;
      }
    }

    uint16_t CRC_16 = generate_CRC_16_bit();
    uint8_t CRC_SIG = CRC_16 >> 8;
    uint8_t CRC_LST = CRC_16 % 256;
    // if (IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 1] == CRC_LST && IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 2] == CRC_SIG) {
    if (IR_module_buffer[NUMBER_OF_PACKAGE_BYTES - 1] == CRC_SIG) {      
      // Serial.print(String(shift_index)+" -> ");
      for (uint8_t i = 0; i < NUMBER_OF_PACKAGE_BYTES; i++) {
        // Serial.print(IR_module_buffer[i]);
        // Serial.print(' ');
      }
      // Serial.println();
      return 1;  //Package is valid. return 1
    } else {
      // Serial.println("Package corrupted");
      return 2;  // CRC check is failed. return 2
    }
  }

  return 0;  // No signal is detected. return 0
}

