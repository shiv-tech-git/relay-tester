/*
 * uart.h
 *
 * Created: 24-Jul-19 3:08:52 PM
 *  Author: Илья
 */ 


#ifndef UART_H_
#define UART_H_
#include <avr/io.h>

void uart_init(uint16_t baud_rate);
void uart_print_string(char* string);
void uart_print_string_ln(char* string);
void uart_print_char(char c);
void uart_print_int(int i);
void uart_print_int_ln(int number);
void uart_print_string_flash(const char* string);


#endif /* UART_H_ */