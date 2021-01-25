/*
 * uart.c
 *
 * Created: 24-Jul-19 3:08:40 PM
 *  Author: Илья
 */ 


#include "uart.h"
#include "global.h"				//F_CPU
#include <avr/io.h>
#include <avr/pgmspace.h>

void uart_init(uint16_t baud_rate){
	
	uint16_t baud_divider = F_CPU/16/baud_rate - 1;
	
	UBRRH = baud_divider >> 8;
	UBRRL = baud_divider;
	
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // Set frame format: 8data, 2stop bit
	
	 
}

void uart_print_string(char* string){
	while(*string != '\0'){
		while (!(UCSRA & (1 << UDRE)))
		;		
		UDR = *string++;
	}
}

void uart_print_char(char c){
		
		while (!(UCSRA & (1 << UDRE)))
		;
		
		UDR = c;
}

void uart_print_int(int number){
	
	char temp_string[6];												//buffer 5 digits for max value of int + '\0'
	int i = 0;
	
	do{
		temp_string[i] = (number%10 + '0');								//int to reverse string
		i++;
		number/=10;
	}while (number > 0);
	
	
	temp_string[i] = '\0';
		
	int length;
	for (length = 0; temp_string[length] != '\0'; length++)				//finding length 
		;
		
	int j;
	char temp_char;
	
	for (i = 0, j = length-1; i < j; i++, j--){							//reverse string
		temp_char = temp_string[i];
		temp_string[i] = temp_string[j];
		temp_string[j] = temp_char;
	}
	
	uart_print_string(temp_string);
	
	
}


void uart_print_string_flash(const char* string){
	while (pgm_read_byte(string) != '\0'){
		uart_print_char(pgm_read_byte(string));
		string++;
	}
}


void uart_print_int_ln(int number){
	uart_print_int(number);
	uart_print_char('\r');
}

void uart_print_string_ln(char* string){
	uart_print_string(string);
	uart_print_char('\r');
}
