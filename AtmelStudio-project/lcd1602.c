/*
 * lcd1602.c
 *
 * Created: 22-Jul-19 2:53:34 PM
 *  Author: SHIV
 */ 
#include "lcd1602.h"
#include "global.h"				//F_CPU
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


#include "uart.h"

//inner functions
static void wait_busy();
static void set_CGRAM_pointer(uint8_t adr);
static void set_DDRAM_pointer(uint8_t adr);
static void write(uint8_t info_type, uint8_t cmd);


//for 4 pin connection

#ifdef _8_PIN_CONNECTION
void lcd_init(){
	CMD_DDR |= 1 << RS | 1 << RW | 1 << E;
	CMD_PORT &= ~(1 << RS | 1 << RW | 1 << E);
	write(CMD, LCD_F | LCD_F_8B | LCD_F_2L);
	write(CMD, LCD_CLR);
	write(CMD, LCD_ENTRY_MODE | LCD_ENTRY_INC);
	write(CMD, LCD_ON | LCD_ON_DISPLAY);
	write(CMD, LCD_HOME);
	
}

static void write(uint8_t info_type, uint8_t cmd){
	wait_busy();
	//_delay_ms(15);
	if (info_type == CMD)
	CLR_BIT(RS)
	else if (info_type == DATA)
	SET_BIT(RS)
	
	CLR_BIT(RW)
	SET_BIT(E)
	
	DATA_DDR = 0xFF;
	DATA_PORT = cmd;
	WAIT
	CLR_BIT(E)
	
	DATA_DDR = 0;
	DATA_PORT = 0xff;
	
}

uint8_t read_byte(){
	
	uint8_t temp;

	DATA_DDR &= ~0xff;				//INPUT
	DATA_PORT |= 0xff;				//PULLUP
	
	CLR_BIT(RS)
	SET_BIT(RW)
	
	_delay_us(1);
	SET_BIT(E)
	_delay_us(1);
	//temp = DATA_PIN << 4;
	temp = DATA_PIN;
	CLR_BIT(E)
	
	CLR_BIT(RW)
	
	//DATA_DDR = 0xff;
	//DATA_PORT = 0;
	
	DATA_DDR |= 0xff;							//INPUT
	DATA_PORT &= ~ 0xff;
	
	
	return temp;
	
}

#endif 

#ifdef _4_PIN_CONNECTION

void write_4_bits_in_reg(volatile uint8_t *reg, uint8_t data){
	
	uint8_t temp = *reg & (0xf << (4 - SHIFT));
	temp |= data << SHIFT;
	*reg = temp;
}

void init_4bit(){
	wait_busy();
	//_delay_ms(15);
	CLR_BIT(RS)
	CLR_BIT(RW)
	//DATA_DDR = 0b1111 << SHIFT;
	write_4_bits_in_reg(&DATA_DDR, 0xf);
	
	SET_BIT(E)
	//DATA_PORT |= (LCD_F >> 4) << SHIFT;
	write_4_bits_in_reg(&DATA_PORT, (LCD_F >> 4));
	_delay_us(1);
	CLR_BIT(E)
}

void lcd_init(){
	CMD_DDR |= 1 << RS | 1 << RW | 1 << E;
	CMD_PORT &= ~(1 << RS | 1 << RW | 1 << E);
	init_4bit();
	write(CMD, LCD_F | LCD_F | LCD_F_2L);
	write(CMD, LCD_CLR);
	write(CMD, LCD_ENTRY_MODE | LCD_ENTRY_INC);
	write(CMD, LCD_ON | LCD_ON_DISPLAY);
	write(CMD, LCD_HOME);
	
}

void write(uint8_t info_type, uint8_t info){
	
	uint8_t high = info >> 4;
	uint8_t low = info & 0xf;
	
	wait_busy();
	//_delay_ms(15);
	
	if (info_type == CMD)
	CLR_BIT(RS)
	else if (info_type == DATA)
	SET_BIT(RS)
	CLR_BIT(RW)
	//DATA_DDR = 0xf << SHIFT;
	write_4_bits_in_reg(&DATA_DDR, 0xf);
	
	//High bits
	SET_BIT(E)
	//DATA_PORT = high << SHIFT;
	write_4_bits_in_reg(&DATA_PORT, high);
	_delay_us(1);
	CLR_BIT(E)
	
	//Low bits
	SET_BIT(E)
	//DATA_PORT = low << SHIFT;
	write_4_bits_in_reg(&DATA_PORT, low);
	_delay_us(1);
	CLR_BIT(E)
	
	DATA_DDR &= ~(0xf << SHIFT);							//INPUT
	DATA_PORT |= (0xf << SHIFT);
	
}

uint8_t read_byte(){
	
	uint8_t temp;
	
	//DATA_DDR = 0;
	//DATA_PORT = 0xff;
	
	DATA_DDR &= ~(0xf << SHIFT);				//INPUT
	DATA_PORT |= (0xf << SHIFT);				//PULLUP
	
	CLR_BIT(RS)
	SET_BIT(RW)
	
	_delay_us(1);
	SET_BIT(E)
	_delay_us(1);
	//temp = DATA_PIN << 4;
	temp = (DATA_PIN >> SHIFT) << 4;
	CLR_BIT(E)
	_delay_us(1);
	SET_BIT(E)
	//temp |= (DATA_PIN & 0b1111);
	temp |= ((DATA_PIN >> SHIFT) & 0xf);
	CLR_BIT(E)
	
	CLR_BIT(RW)
	
	//DATA_DDR = 0xff;
	//DATA_PORT = 0;
	
	//DATA_DDR |= (0xf << SHIFT);							//INPUT
	//DATA_PORT &= ~ (0xf << SHIFT);
	
	return temp;
	
}

#endif

void lcd_print_char(char c){
	write(DATA, c);
}

void lcd_print_string(char* string){
   while(*string != '\0'){
      write(DATA, *string);
      string++;
   }
}

void lcd_create_symbol(uint8_t symbol[8], int adr){
   int i;
   adr *= 8;
   set_CGRAM_pointer(adr);
   
   for(i = 0; i < 8; i++){
      write(DATA, symbol[i]);
   }
      
}

void lcd_print_symbol(uint8_t address){
	write(DATA, address);
}

void lcd_clear(){
   write(CMD, LCD_CLR);
   write(CMD, LCD_HOME);
}

void lcd_home(){
   write(CMD, LCD_HOME);
}

void lcd_set_position(uint8_t row, uint8_t column){
   set_DDRAM_pointer(64*row + column);
}

void lcd_shift(uint8_t direction){
   write(CMD, LCD_MOVE | LCD_MOVE_DISP | direction);
}

void lcd_pointer_settings(uint8_t position_state, uint8_t blinking_state){
	write(CMD, LCD_ON | LCD_ON_DISPLAY | position_state | blinking_state);
}

void lcd_print_int(int number){
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
	
	lcd_print_string(temp_string);
}

void lcd_print_string_flash(const char* string){
	while (pgm_read_byte(string) != '\0'){
		lcd_print_char(pgm_read_byte(string));
		string++;
	}
}



static void wait_busy(){
	while(read_byte() & (1 << 7));
}

static void set_DDRAM_pointer(uint8_t adr){
	write(CMD, LCD_DDRAM | adr);
}

static void set_CGRAM_pointer(uint8_t adr){
	write(CMD, LCD_CGRAM | adr);
}