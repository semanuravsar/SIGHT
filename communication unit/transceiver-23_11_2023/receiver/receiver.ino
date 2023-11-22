#define IR_RECEIVE_PIN 3
#define HALF_PERIOD_US 600
#define NUMBER_OF_PACKAGE_BITS 16
#define BAUD_RATE 115200


void setup() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  Serial.begin(BAUD_RATE);
}

void loop() { 
  count_IR();
}


uint8_t last_received_val = 0;
void count_IR(){
  if(0 == digitalRead(IR_RECEIVE_PIN)){
    delayMicroseconds(3*HALF_PERIOD_US);
    uint8_t data_buffer[NUMBER_OF_PACKAGE_BITS]; //0:MSB, 7:LSB

    for(uint8_t i = 0; i<NUMBER_OF_PACKAGE_BITS;i++){
      data_buffer[i]= digitalRead(IR_RECEIVE_PIN);
      delayMicroseconds(2*HALF_PERIOD_US);
    }

      uint8_t received_val = data_buffer[0]*128+data_buffer[1]*64+data_buffer[2]*32+data_buffer[3]*16+data_buffer[4]*8+data_buffer[5]*4+data_buffer[6]*2+data_buffer[7];
      uint8_t inverted_received_val = data_buffer[8]*128+data_buffer[9]*64+data_buffer[10]*32+data_buffer[11]*16+data_buffer[12]*8+data_buffer[13]*4+data_buffer[14]*2+data_buffer[15];

      if(received_val + inverted_received_val == 255){
        Serial.println(received_val);
      }
    
  }

}




