#define IR_LED 5
void send_IR(uint8_t);

void setup(){
  pinMode(IR_LED, OUTPUT);
}

void loop(){
  // for(int i = 1; i<10; i++){
  //   send_IR(i);
  // }
  send_IR(5);
  send_IR(3);
  send_IR(7);
  send_IR(1);
  send_IR(1);
  send_IR(7);
  send_IR(3);
  send_IR(5);
  delay(2000);
}


void send_IR(uint8_t val_to_send){
  //total required time = val_to_send * 1.2ms
  for (int k=0;k<(val_to_send);k++){
      unsigned long start_time = micros();
      while(micros()-start_time <600){
        digitalWrite(IR_LED, LOW);
        delayMicroseconds(10);
        digitalWrite(IR_LED, HIGH);
        delayMicroseconds(10);
      }
      delayMicroseconds(600);
  }
  delay(20);
}

