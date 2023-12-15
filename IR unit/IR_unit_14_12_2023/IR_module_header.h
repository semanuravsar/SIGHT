// IRModule.h

#ifndef IR_MODULE_H
#define IR_MODULE_H

#include <Arduino.h>

#define IR_RECEIVE_PIN 3
#define IR_LED 5

#define NUMBER_OF_PACKAGE_BYTES 4

#define K_NUMBER_OF_BURSTS 60
#define BURST_HALF_PERIOD_US 13
#define LISTEN_DURATION_MS 20

extern unsigned long TRIGGER_DURATION_US;
extern uint8_t IR_module_buffer[NUMBER_OF_PACKAGE_BYTES];

void initialize_IR_module();
void set_buffer(uint16_t byte_index, uint8_t byte_value);
void transmit_buffer();
void transmit_zero();
void transmit_one();
void listen_IR();
uint16_t generate_CRC_16_bit();
uint16_t CRC_16_bit_for_1BYTE(uint16_t data, uint16_t last_data);

#endif // IR_MODULE_H
