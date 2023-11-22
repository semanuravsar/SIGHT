#define IR_RECEIVE_PIN 3
#define BAUD_RATE 115200

void setup() {
  pinMode(IR_RECEIVE_PIN, INPUT);
  Serial.begin(BAUD_RATE);
}

void loop() { 
  count_IR();
}


void count_IR(){
  int transitionCount = 0;  // Counter for the number of high-to-low transitions

  if(0 == digitalRead(IR_RECEIVE_PIN)){
    uint8_t last_IR_state = 0;
    unsigned long start_time = millis();
    while(millis()-start_time<18){
      uint8_t IR_read =  digitalRead(IR_RECEIVE_PIN);
      if(last_IR_state != IR_read){
          last_IR_state = IR_read;
          transitionCount = transitionCount +1;
      }
    }
    transitionCount = (transitionCount +1)>>1;
    Serial.println(transitionCount);
  }

}




