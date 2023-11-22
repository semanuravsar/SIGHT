#include <IRremote.h>

// Define the IR LED pin
const int irLedPin = 8; // Change this to the appropriate pin number

// Define the custom IR code (you can replace this with your own code)
unsigned long customCode = 0x20DF40BF;

void setup() {
  // Initialize the IR LED pin
  pinMode(irLedPin, OUTPUT);

}

void loop() {
  // Send the custom IR code
  IrSender.sendNEC(customCode, 32); // 32 is the number of bits
  // Wait for a moment before sending the next code
  delay(2); // 5-second delay (adjust as needed)
}
