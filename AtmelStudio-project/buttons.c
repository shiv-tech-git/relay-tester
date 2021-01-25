/*
 * buttons_v2.c
 *
 * Created: 31-Jul-19 1:08:34 PM
 *  Author: Илья
 */ 

#include "buttons.h"
#include "uart.h"

/* for Atmega8 20 ms timer
#define PERIOD_20_MS_BOTTON_VALUE 100
volatile uint8_t timer0_flag;

ISR (TIMER0_OVF_vect){
	TCNT0 = PERIOD_20_MS_BOTTON_VALUE;
	timer0_flag = 1;
}*/

#define LONG_PRESS_VALUE 20

static uint8_t buttons_virtual_PIN();


struct buttons_virtual_port_struct{
	volatile uint8_t* b_pin;
	uint8_t b_bit;
}buttons_virtual_port[BUTTON_AMOUNT];

struct buttons_status_struct{
	uint8_t last_state;
	uint8_t pressed;
	uint8_t just_released; 
	uint8_t short_press;
	uint8_t long_press;
	uint8_t holding;
	uint8_t long_press_counter;
	}buttons_status;
	
static uint8_t buttons_virtual_PIN(){
	
	uint8_t virtual_PIN = 0;															//virtual PINx register
	
	for (int i = BUTTON_AMOUNT - 1; i >= 0; i--){
		virtual_PIN <<= 1;
		if(*buttons_virtual_port[i].b_pin & (1 << buttons_virtual_port[i].b_bit))		//collecting statuses of all PINx bits of all ports
		virtual_PIN |= 1;																//putting this stuff in our virtual register
	}
	
	return virtual_PIN;
}

void buttons_scanner(){
	
	static uint8_t debounce_counter_0 = 0xFF;											//vertical 2 bit counter
	static uint8_t debounce_counter_1 = 0xFF;
	static uint8_t temp;
	
	temp = ~buttons_virtual_PIN();														//reading the virtual port (active low)			1 - button is pressed
	temp ^= buttons_status.last_state;													//checking changes from last time				1 - status was changed
	
	debounce_counter_0 = ~(debounce_counter_0 & temp);									//decreasing counter if was change				
	debounce_counter_1 = debounce_counter_1 ^ (debounce_counter_0 & temp);				//temp == 0  then counter = 0xFF
	
	temp &= debounce_counter_0 & debounce_counter_1;									//if counter == 0xFF then fix the change after debouncing
	buttons_status.last_state ^= temp;													//invert last status if was change
	
	buttons_status.pressed |= buttons_status.last_state & temp;							//if was change and status == 1 then button was pressed (after debouncing)
	buttons_status.just_released |= ~buttons_status.last_state & temp;					//if was change and status == 0 then button was released (after debouncing)

	//short press handler
	buttons_status.short_press = (buttons_status.pressed & buttons_status.just_released) & ~buttons_status.holding;				//if button is pressed already and it just released then short press happened
	buttons_status.pressed ^= buttons_status.pressed & buttons_status.just_released;											//if button is pressed already and it just released then button not pressed any more
	buttons_status.holding ^= buttons_status.holding & buttons_status.just_released;											//if button is long pressed already and it just released then button not long pressed any more
	buttons_status.just_released &= ~buttons_status.just_released;																//if button just released and we handle it then released status removed

	
	if(temp)
		buttons_status.long_press_counter = 0;
	
	buttons_status.long_press_counter++;
	
	if (buttons_status.long_press_counter > LONG_PRESS_VALUE){
		buttons_status.long_press_counter = 0;
		buttons_status.long_press |= buttons_status.pressed & ~buttons_status.holding;
		buttons_status.holding |= buttons_status.pressed;
	}
		
	
}

uint8_t buttons_check_short_press(uint8_t button_mask){
	
	button_mask &= buttons_status.short_press;											//checking our button
	buttons_status.short_press &= ~button_mask;											//if it was pressed, then clear the bit
	
	return button_mask;
}

uint8_t buttons_check_long_press(uint8_t button_mask){
	
	button_mask &= buttons_status.long_press;											//checking our button
	buttons_status.long_press &= ~button_mask;
	
	return button_mask;
}

uint8_t buttons_check_holding(uint8_t button_mask){
	
	button_mask &= buttons_status.holding;
	return button_mask;
}


void buttons_init_hardware(){
	
	//button 0
	#if BUTTON_AMOUNT >= 1
		BUTTON_0_PORT |= 1 << BUTTON_0_BIT;
		BUTTON_0_DDR |= 0 << BUTTON_0_BIT;
		buttons_virtual_port[0].b_pin = &BUTTON_0_PIN;
		buttons_virtual_port[0].b_bit = BUTTON_0_BIT;
	#endif
	
	//button 1
	#if BUTTON_AMOUNT >= 2
		BUTTON_1_PORT |= 1 << BUTTON_1_BIT;
		BUTTON_1_DDR |= 0 << BUTTON_1_BIT;
		buttons_virtual_port[1].b_pin = &BUTTON_1_PIN;
		buttons_virtual_port[1].b_bit = BUTTON_1_BIT;
	#endif
	
	//button 2
	#if BUTTON_AMOUNT >= 3
		BUTTON_2_PORT |= 1 << BUTTON_2_BIT;
		BUTTON_2_DDR |= 0 << BUTTON_2_BIT;
		buttons_virtual_port[2].b_pin = &BUTTON_2_PIN;
		buttons_virtual_port[2].b_bit = BUTTON_2_BIT;
	#endif
	
	//button 3
	#if BUTTON_AMOUNT >= 4
		BUTTON_3_PORT |= 1 << BUTTON_3_BIT;
		BUTTON_3_DDR |= 0 << BUTTON_3_BIT;
		buttons_virtual_port[3].b_pin = &BUTTON_3_PIN;
		buttons_virtual_port[3].b_bit = BUTTON_3_BIT;
	#endif
	
	//button 4
	#if BUTTON_AMOUNT >= 5
		BUTTON_4_PORT |= 1 << BUTTON_4_BIT;
		BUTTON_4_DDR |= 0 << BUTTON_4_BIT;
		buttons_virtual_port[4].b_pin = &BUTTON_4_PIN;
		buttons_virtual_port[4].b_bit = BUTTON_4_BIT;
	#endif
	
	//button 5
	#if BUTTON_AMOUNT >= 6
		BUTTON_5_PORT |= 1 << BUTTON_5_BIT;
		BUTTON_5_DDR |= 0 << BUTTON_5_BIT;
		buttons_virtual_port[5].b_pin = &BUTTON_5_PIN;
		buttons_virtual_port[5].b_bit = BUTTON_5_BIT;
	#endif
	
	//button 6
	#if BUTTON_AMOUNT >= 7
		BUTTON_6_PORT |= 1 << BUTTON_6_BIT;
		BUTTON_6_DDR |= 0 << BUTTON_6_BIT;
		buttons_virtual_port[6].b_pin = &BUTTON_6_PIN;
		buttons_virtual_port[6].b_bit = BUTTON_6_BIT;
	#endif
	
	//button 7
	#if BUTTON_AMOUNT >= 8
		BUTTON_7_PORT |= 1 << BUTTON_7_BIT;
		BUTTON_7_DDR |= 0 << BUTTON_7_BIT;
		buttons_virtual_port[7].b_pin = &BUTTON_7_PIN;
		buttons_virtual_port[7].b_bit = BUTTON_7_BIT;
	#endif
	
	
}