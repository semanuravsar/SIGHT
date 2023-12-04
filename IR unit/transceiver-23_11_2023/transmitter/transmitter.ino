#define IR_LED 5
#define HALF_PERIOD_US 600
#define NUMBER_OF_PACKAGE_BITS 16

void send_IR(uint8_t);

void setup(){
  pinMode(IR_LED, OUTPUT);
  Serial.begin(115200);
}

void loop(){
  // for(int i = 1; i<10; i++){
  //   send_IR(i);
  // }
  
  for(int i =0; i<255; i++){
  send_byte(i);
  delay(50);
  }

}

void send_byte(uint8_t byte_value){
  send_IR_bit(0);//start bit
  for (int i = 7; i >= 0; i--) {
    send_IR_bit((byte_value >> i) & 1);    
  }

  uint8_t inverter_byte_value = ~byte_value;
  for (int i = 7; i >= 0; i--) {
    send_IR_bit((inverter_byte_value >> i) & 1);    
  }
}
void send_IR_bit(uint8_t val_to_send){
  
      if(val_to_send == 0){
        unsigned long start_time = micros();
        while(micros()-start_time <HALF_PERIOD_US){
          digitalWrite(IR_LED, LOW);
          delayMicroseconds(10);
          digitalWrite(IR_LED, HIGH);
          delayMicroseconds(10);
        }
        delayMicroseconds(HALF_PERIOD_US);
      }else{
        delayMicroseconds(2*HALF_PERIOD_US);
      }


      
}

