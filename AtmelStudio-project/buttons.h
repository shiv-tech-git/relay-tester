/*
 * buttons.h
 *
 * Created: 25-Jul-19 3:40:29 PM
 *  Author: Илья
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>

#define BUTTON_AMOUNT 4




//button 0
#define UP_BUTTON				(1 << 0)		//button name
#define BUTTON_0_PORT_LETTER	C				//button port
#define BUTTON_0_BIT			2				//button pin

//button 1
#define DOWN_BUTTON				(1 << 1)
#define BUTTON_1_PORT_LETTER	B
#define BUTTON_1_BIT			5

//button 2
#define LEFT_BUTTON				(1 << 2)
#define BUTTON_2_PORT_LETTER	C
#define BUTTON_2_BIT			0

//button 3
#define RIGHT_BUTTON			(1 << 3)
#define BUTTON_3_PORT_LETTER	C
#define BUTTON_3_BIT			1

//button 4
#define NAME4					(1 << 4)
#define BUTTON_4_PORT_LETTER	B
#define BUTTON_4_BIT			4

//button 5
#define NAME5					(1 << 5)
#define BUTTON_5_PORT_LETTER	B
#define BUTTON_5_BIT			5

//button 6
#define NAME6					(1 << 6)
#define BUTTON_6_PORT_LETTER	B
#define BUTTON_6_BIT			6

//button 7
#define NAME7					(1 << 7)
#define BUTTON_7_PORT_LETTER	B
#define BUTTON_7_BIT			7



#define GET_REG_NAME(r, l) r##l
#define GET_REG_NAME_(r,l) GET_REG_NAME(r, l)

//button 0
#define BUTTON_0_PORT	GET_REG_NAME_(PORT, BUTTON_0_PORT_LETTER)
#define BUTTON_0_DDR	GET_REG_NAME_(DDR, BUTTON_0_PORT_LETTER)
#define BUTTON_0_PIN	GET_REG_NAME_(PIN, BUTTON_0_PORT_LETTER)
//button 1
#define BUTTON_1_PORT	GET_REG_NAME_(PORT, BUTTON_1_PORT_LETTER)
#define BUTTON_1_DDR	GET_REG_NAME_(DDR, BUTTON_1_PORT_LETTER)
#define BUTTON_1_PIN	GET_REG_NAME_(PIN, BUTTON_1_PORT_LETTER)
//button 2
#define BUTTON_2_PORT	GET_REG_NAME_(PORT, BUTTON_2_PORT_LETTER)
#define BUTTON_2_DDR	GET_REG_NAME_(DDR, BUTTON_2_PORT_LETTER)
#define BUTTON_2_PIN	GET_REG_NAME_(PIN, BUTTON_2_PORT_LETTER)
//button 3
#define BUTTON_3_PORT	GET_REG_NAME_(PORT, BUTTON_3_PORT_LETTER)
#define BUTTON_3_DDR	GET_REG_NAME_(DDR, BUTTON_3_PORT_LETTER)
#define BUTTON_3_PIN	GET_REG_NAME_(PIN, BUTTON_3_PORT_LETTER)
//button 4
#define BUTTON_4_PORT	GET_REG_NAME_(PORT, BUTTON_4_PORT_LETTER)
#define BUTTON_4_DDR	GET_REG_NAME_(DDR, BUTTON_4_PORT_LETTER)
#define BUTTON_4_PIN	GET_REG_NAME_(PIN, BUTTON_4_PORT_LETTER)
//button 5
#define BUTTON_5_PORT	GET_REG_NAME_(PORT, BUTTON_5_PORT_LETTER)
#define BUTTON_5_DDR	GET_REG_NAME_(DDR, BUTTON_5_PORT_LETTER)
#define BUTTON_5_PIN	GET_REG_NAME_(PIN, BUTTON_5_PORT_LETTER)
//button 6
#define BUTTON_6_PORT	GET_REG_NAME_(PORT, BUTTON_6_PORT_LETTER)
#define BUTTON_6_DDR	GET_REG_NAME_(DDR, BUTTON_6_PORT_LETTER)
#define BUTTON_6_PIN	GET_REG_NAME_(PIN, BUTTON_6_PORT_LETTER)
//button 7
#define BUTTON_7_PORT	GET_REG_NAME_(PORT, BUTTON_7_PORT_LETTER)
#define BUTTON_7_DDR	GET_REG_NAME_(DDR, BUTTON_7_PORT_LETTER)
#define BUTTON_7_PIN	GET_REG_NAME_(PIN, BUTTON_7_PORT_LETTER)



void buttons_init_hardware();
void buttons_scanner();
uint8_t buttons_check_short_press(uint8_t button_mask);
uint8_t buttons_check_long_press(uint8_t button_mask);
uint8_t buttons_check_holding(uint8_t button_mask);


#endif /* BUTTONS_H_ */