/*
 * lcd1602.h
 *
 * Created: 24-Jul-19 11:19:44 AM
 *  Author: Илья
 */ 


#ifndef LCD1602_H_
#define LCD1602_H_

#include <avr/io.h>

//========================THIS SHOULD BE TUNED=======================//

#define _4_PIN_CONNECTION
//#define _8_PIN_CONNECTION

//for 4 bit connecting
//#define SHIFT HIGH_BITS					//<===== set this
#define SHIFT LOW_BITS						//do you want use lower 4 bits of data register or high 4 bits.

//HARDWARE 
#define DATA_PORT		PORTD
#define DATA_DDR		DDRD
#define DATA_PIN		PIND

#define CMD_PORT		PORTC
#define CMD_DDR			DDRC
#define CMD_PIN			PINC

#define RS 3
#define RW 4
#define E 5




//========================THIS SHOULD BE TUNED=======================//


//type of connection
#define LCD_4 4
#define LCD_8 8

//Shift
#define LOW_BITS 0
#define HIGH_BITS 4



//modes for write function
#define CMD 0
#define DATA 1

//SET & CLR bit in command port
#define SET_BIT(BIT) CMD_PORT |= (1 << BIT);
#define CLR_BIT(BIT) CMD_PORT &= ~(1 << BIT);

//delay
#define STROBE 200
#define WAIT for(int i = STROBE; i > 0; i--);



//clear display
#define LCD_CLR				(1 << 0)
//return home
#define LCD_HOME			(1 << 1)
//entry mode set
#define LCD_ENTRY_MODE		(1 << 2)
#define LCD_ENTRY_INC		(1 << 1)
#define LCD_ENTRY_SHIFT		(1 << 0)
//display ON/OFF control
#define LCD_ON				(1 << 3)
#define LCD_ON_DISPLAY		(1 << 2)
#define LCD_ON_CURSOR		(1 << 1)
#define LCD_ON_BLINK		(1 << 0)
//for pointer settings function
#define POSITION_ON			(1 << 1)
#define POSITION_OFF		(0 << 1)
#define BLINKING_ON			(1 << 0)
#define BLINKING_OFF		(0 << 0)
//cursor or display shift
#define LCD_MOVE			(1 << 4)
#define LCD_MOVE_DISP		(1 << 3)
#define LCD_MOVE_RIGHT		(1 << 2)
//direction for shift function
#define LEFT				(0 << 2)
#define RIGHT				(1 << 2)
//function set
#define LCD_F				(1 << 5)
#define LCD_F_8B			(1 << 4)
#define LCD_F_2L			(1 << 3)
#define LCD_F_10D			(1 << 2)
//set CGRAM
#define LCD_CGRAM			(1 << 6)
//set DDRAM
#define LCD_DDRAM			(1 << 7)

//busy bite
#ifdef _4_PIN_CONNECTION
	#if(SHIFT == LOW_BITS)
		#define BUSY_BIT			(1 << 3)
	#endif
	#if(SHIFT == HIGH_BITS)
		#define BUSY_BIT			(1 << 7)
	#endif	
#endif

#ifdef _8_PIN_CONNECTION
	#define BUSY_BIT				(1 << 7)
#endif


void lcd_init();
void lcd_print_char(char c);
void lcd_print_string(char* string);
void lcd_clear();
void lcd_home();
void lcd_set_position(uint8_t row, uint8_t column);
void lcd_pointer_settings(uint8_t position_state, uint8_t blinking_state);
void lcd_shift(uint8_t direction);
void lcd_print_int(int number);
void lcd_print_string_flash(const char* string);
void lcd_create_symbol(uint8_t symbol[8], int adr);
void lcd_print_symbol(uint8_t);



#endif /* LCD1602_H_ */