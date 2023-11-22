#include <IRremote.h> 

IRrecv irrecv(2); 
decode_results results;
void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600); 
}

int is_receiving = 0;
int number_of_received_datas = 0;
unsigned long last_time_received = 0;
void loop() {
  if (irrecv.decode(&results)){ 
    if (is_receiving == 0){
      is_receiving = 1;
      number_of_received_datas = 1;
      last_time_received = millis();
    }else{
      number_of_received_datas = number_of_received_datas +1;
    }
    //Serial.println(results.value, HEX); 
    delay(100);
    irrecv.resume();
  } 

  if(is_receiving == 1 && millis()-last_time_received > 1250){
    is_receiving = 0;
    //Serial.println(number_of_received_datas);    
    if(number_of_received_datas == 2){
      Serial.println("Hello");
    }else if(number_of_received_datas == 3){
      Serial.println("World !");
    }else if(number_of_received_datas == 1){
      Serial.println("");
    }
    number_of_received_datas = 0;
  }
}


