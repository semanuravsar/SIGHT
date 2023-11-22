// IR Transmitter ...
#include <IRremote.h>

IRsend irsend;

void setup()
{
  pinMode(7,INPUT); // button 1
  pinMode(8,INPUT); // button 2
  pinMode(9,INPUT); // button 3
  
}

void loop() {

	if (digitalRead(7) == HIGH){
   
  delay(50);
  irsend.sendNEC(0x34895725, 32);} 

  if (digitalRead(8) == HIGH){
  
  delay(50);
  irsend.sendNEC(0x56874159, 32);} 

  if (digitalRead(9) == HIGH){
 
  delay(50);
  irsend.sendNEC(0x15467823, 32);} 
  
   delay(1);
  
	
	
	}
