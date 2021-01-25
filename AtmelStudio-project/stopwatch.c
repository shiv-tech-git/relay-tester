/*
 * stopwatch.c
 *
 * Created: 02-Sep-19 3:42:34 PM
 *  Author: Илья
 */ 

#include <stdint.h>
#include "timer.h"
#include "uart.h"
#include "menu.h"
#include "lcd1602.h"
#include "stopwatch.h"
#include "buttons.h"

#define DEBOUNCE_VALUE 10

#define K1_PORT	PORTD
#define K1_DDR	DDRD
#define K1_PIN	PIND
#define K1_BIT	6

#define K2_PORT	PORTD
#define K2_DDR	DDRD
#define K2_PIN	PIND
#define K2_BIT	5

#define U1_PORT	PORTD
#define U1_DDR	DDRD
#define U1_PIN	PIND
#define U1_BIT	7

#define U2_PORT	PORTB
#define U2_DDR	DDRB
#define U2_PIN	PINB
#define U2_BIT	0




#define PIN_CHECK(pin, bit) (pin & (1 << bit))

#define ONCE			0
#define CONTINUE		1
#define NOPE			3

#define _01				1
#define _10				0
#define DEBOUNCE		1
#define NO_DEBOUNCE		0

#define UP_BUTTON_PIN	BUTTON_0_PIN
#define UP_BUTTON_BIT	BUTTON_0_BIT

#define START_STATES 0
#define STOP_STATES 1

#define PRINT_ERROR(number, string)	lcd_clear();\
									lcd_print_string_flash(PSTR("Error#"));\
									lcd_print_int(number);\
									lcd_set_position(1,0);\
									lcd_print_string_flash(PSTR(#string));
									
uint8_t (*condition_pointer[9])(uint8_t condition);

struct time_struct last_measured_time = {0, 0, 0};


uint8_t check_condition(volatile uint8_t *reg, uint8_t bit, uint8_t f_state, uint8_t deb, uint8_t stopwatch_state){
		
	static uint8_t state[2];
	static uint8_t last_state[2];
	
	state[stopwatch_state] = !PIN_CHECK(*reg, bit);																				//reverse state (PULL_UP)
	
	if (deb){																												//for debounce mode
		if ((f_state ? state[stopwatch_state] : !state[stopwatch_state]) && (last_state[stopwatch_state] != state[stopwatch_state])){				//if finite state _10 (0) then we need active low state (invert state)
			last_state[stopwatch_state] = state[stopwatch_state];
			return CONTINUE;
		} else {
			last_state[stopwatch_state] = state[stopwatch_state];
			return NOPE;
		}
	} else {
		if (f_state ? state[stopwatch_state] : !state[stopwatch_state]){
			return ONCE;
		} else {
			return NOPE;
		}
	}
	
}


uint8_t condition_K1_01(uint8_t stopwatch_state){
	return check_condition(&K1_PIN, K1_BIT, _01, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_K1_01d(uint8_t stopwatch_state){
	return check_condition(&K1_PIN, K1_BIT, _01, DEBOUNCE, stopwatch_state);
}

uint8_t condition_K1_10(uint8_t stopwatch_state){
	return check_condition(&K1_PIN, K1_BIT, _10, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_K2_01(uint8_t stopwatch_state){
	return check_condition(&K2_PIN, K2_BIT, _01, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_K2_01d(uint8_t stopwatch_state){
	return check_condition(&K2_PIN, K2_BIT, _01, DEBOUNCE, stopwatch_state);
}

uint8_t condition_K2_10(uint8_t stopwatch_state){
	return check_condition(&K2_PIN, K2_BIT, _10, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_U1_01(uint8_t stopwatch_state){
	return check_condition(&U1_PIN, U1_BIT, _01, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_U1_10(uint8_t stopwatch_state){
	return check_condition(&U1_PIN, U1_BIT, _10, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_U2_01(uint8_t stopwatch_state){
	return check_condition(&U2_PIN, U2_BIT, _01, NO_DEBOUNCE, stopwatch_state);
}

uint8_t condition_U2_10(uint8_t stopwatch_state){
	return check_condition(&U2_PIN, U2_BIT, _10, NO_DEBOUNCE, stopwatch_state);
}



void stopwatch_init(){
	
	K1_DDR &= ~(1 << K1_BIT);
	K1_PORT |= (1 << K1_BIT);
	
	K2_DDR &= ~(1 << K2_BIT);
	K2_PORT |= (1 << K2_BIT);
	
	U1_DDR &= ~(1 << U1_BIT);
	U1_PORT |= (1 << U1_BIT);
	
	U2_DDR &= ~(1 << U2_BIT);
	U2_PORT |= (1 << U2_BIT);
	
	condition_pointer[K1_01] = condition_K1_01;
	condition_pointer[K1_10] = condition_K1_10;
	condition_pointer[K1_01d] = condition_K1_01d;
	condition_pointer[K2_01] = condition_K2_01;
	condition_pointer[K2_10] = condition_K2_10;
	condition_pointer[K2_01d] = condition_K2_01d;
	condition_pointer[U1_01] = condition_U1_01;
	condition_pointer[U1_10] = condition_U1_10;
	condition_pointer[U2_01] = condition_U2_01;
	condition_pointer[U2_10] = condition_U2_10;

}

uint8_t check_condition_errors(uint8_t cond){

	if(cond == NO_CONDITON){
		PRINT_ERROR(0,Nope condition)
		return 0;
	}
	
	if (condition_codes[START_CONDITION] == condition_codes[STOP_CONDITION]){
		PRINT_ERROR(1,Same conditions)
		return 0;
	}
	
		
	//if(cond == K1_01 || cond == K1_01d){
		//if(!PIN_CHECK(K1_PIN, K1_BIT)){
			//PRINT_ERROR(1,K1 is closed)
			//return 0;
		//}
	//}
	//
	//if(cond == K1_10){
		//if(PIN_CHECK(K1_PIN, K1_BIT)){
			//PRINT_ERROR(2,K1 is open)
			//return 0;
		//}
	//}
	//
	//
	//if(cond == K2_01 || cond == K2_01d){
		//if(!PIN_CHECK(K2_PIN, K2_BIT)){
			//PRINT_ERROR(3,K2 is closed)
			//return 0;
		//}
	//}
	//
	//if(cond == K2_10){
		//if(PIN_CHECK(K2_PIN, K2_BIT)){
			//PRINT_ERROR(4,K2 is open)
			//return 0;
		//}
	//}
	//
	//if(cond == U1_01){
		//if(!PIN_CHECK(U1_PIN, U1_BIT)){
			//PRINT_ERROR(5,U1 is applied)
			//return 0;
		//}
	//}
	//
	//if(cond == U1_10){
		//if(PIN_CHECK(U1_PIN, U1_BIT)){
			//PRINT_ERROR(6,U1 is removed)
			//return 0;
		//}
	//}
	//
	//if(cond == U2_01){
		//if(!PIN_CHECK(U2_PIN, U2_BIT)){
			//PRINT_ERROR(7,U2 is applied)
			//return 0;
		//}
	//}
	//
	//if(cond == U2_10){
		//if(PIN_CHECK(U2_PIN, U2_BIT)){
			//PRINT_ERROR(8,U2 is removed)
			//return 0;
		//}
	//}
	//
	//
	
	return 1;
}

uint8_t stopwatch_error(){
	
	if(!check_condition_errors(condition_codes[START_CONDITION]))
		return 0;
	if(!check_condition_errors(condition_codes[STOP_CONDITION]))
		return 0;
		
	return 1;
}

struct time_struct time_handler(struct timer_value_struct timer1){
	struct time_struct temp;
	
	temp.seconds = timer1.period/20;
	temp.milliseconds = ((timer1.period % 20) * 50000 + timer1.timer_vatue)/1000;
	temp.microseconds = ((timer1.period % 20) * 50000 + timer1.timer_vatue)%1000;

	return temp;
}

inline void conditional_start_stopwatch(uint8_t *start_flag, uint8_t *stop_flag, uint8_t *debounce_flag, uint8_t *debounce_timer){
																					//if start condition checking is allow
		uint8_t condition_check_result = condition_pointer[condition_codes[START_CONDITION]](START_STATES);
		if(condition_check_result == ONCE){																//condition without debounce, run once, start == 0
			//uart_print_string_ln("ONCE start");															//stop flag == 1 -> allow stop condition checking
			*start_flag = 0;
			*stop_flag = 1;
			start_timer1();																				//start timer 1
		} else if(condition_check_result == CONTINUE){													//this part only for debounce
			uart_print_string_ln("CONTINUE start");
			//*start_flag = 1;																				//if we check debounce condition, allow stop condition checking
			//*stop_flag = 0;																				//but continue check start condition. If it repeat, restart timer.
			start_timer1();
			*debounce_flag = 1;																			
			*debounce_timer = DEBOUNCE_VALUE;
		}
}

inline void conditional_stop_stopwatch(uint8_t *stop_flag, uint8_t *end_stopwatch, uint8_t *debounce_flag, uint8_t *debounce_timer, struct time_struct *measured_time){
	
		uint8_t condition_check_result = condition_pointer[condition_codes[STOP_CONDITION]](STOP_STATES);
		
		if(condition_check_result == ONCE){																//catch stop condition
				//uart_print_string_ln("ONCE stop");															//save time
				*measured_time = time_handler(check_timer1());														//stop timer
				stop_timer1();
				*end_stopwatch = 1;																			//exit loop
			} else if (condition_check_result == CONTINUE){													//this part only for debounce
				*measured_time = time_handler(check_timer1());
				//uart_print_string_ln("CONTINUE stop");
				*debounce_flag = 1;																			//catch stop condition in debounce mode
				*debounce_timer = DEBOUNCE_VALUE;																		//start debounce timer
			}
		
}

inline void debounce_timer_stopwatch(uint8_t *start_flag, uint8_t *stop_flag,  uint8_t *debounce_flag, uint8_t *local_timer0_flag, uint8_t *debounce_timer, uint8_t *end_stopwatch){
	
		if(*local_timer0_flag){
			(*debounce_timer)--;	
			uart_print_int_ln(*debounce_timer);																	//decrease debounce timer each 20 ms
		}
		if(*debounce_timer == 0){																	//if debounce timer == 0
			if (*stop_flag){
				stop_timer1();
				*end_stopwatch = 1;
			} else if (*start_flag){
				uart_print_string_ln("stop enable");
				*start_flag = 0;
				*stop_flag = 1;
				*debounce_flag = 0;
			}																//exit loop
			//uart_print_string_ln("end_stopwatch");
		}
	
}

inline void intermediate_time_render(uint8_t *start_flag, uint8_t *local_timer0_flag, struct time_struct *intermediate_time, uint8_t *last_seconds){
	//intermediate time render if counting time more then 1 sec
	
		*intermediate_time = time_handler(check_timer1());											//check time each 20 ms
		
		if(intermediate_time->seconds >= 1 && intermediate_time->seconds > *last_seconds){				//intermediate time render
			*last_seconds = intermediate_time->seconds;
			lcd_set_position(0,5);
			lcd_print_int(intermediate_time->seconds);
		}
	
}


void stopwatch_routine(){
	
	uint8_t start_flag = 1;									//start checking permission flag
	uint8_t stop_flag = 0;									//sop checking permission flag
	uint8_t end_stopwatch = 0;								//when end_stopwatch == 1 -> exit of loop
	uint8_t last_seconds = 0;								//last second for intermediate_time_render
	uint8_t debounce_timer = 0;								
	uint8_t debounce_flag = 0;
	uint8_t local_timer0_flag = 0;
	struct time_struct measured_time = {0,0,0};
	struct time_struct intermediate_time = {0,0,0};
	
	lcd_clear();
	lcd_print_string("Time:0");
	
		
	while (!end_stopwatch){
		
		local_timer0_flag = timer0_check_flag();																		//for measure big time interval
		
		if(start_flag)		conditional_start_stopwatch(&start_flag, &stop_flag, &debounce_flag, &debounce_timer);														//check start condition and start timer
		if(stop_flag)		conditional_stop_stopwatch(&stop_flag, &end_stopwatch, &debounce_flag, &debounce_timer, &measured_time);	//check stop condition and stop timer
		if(debounce_flag)	debounce_timer_stopwatch(&start_flag, &stop_flag, &debounce_flag, &local_timer0_flag, &debounce_timer, &end_stopwatch);				//if stop debounce condition checked -> start debounce timer (if timer value == 0 -> stop main timer, exit loop)
		if(!start_flag && local_timer0_flag) intermediate_time_render(&start_flag, &local_timer0_flag, &intermediate_time, &last_seconds);				//if measured time is longer than 1 second -> print seconds in display
		
		if(!(UP_BUTTON_PIN & (1 << UP_BUTTON_BIT)))																	//emergency exit from loop
			return; 
		
		local_timer0_flag = 0;																						//reset time flag
	}
	
	last_measured_time = measured_time;																				//for last measure menu
	
	lcd_set_position(0,5);
	lcd_print_int(measured_time.seconds);
	lcd_print_char('.');
	if(measured_time.milliseconds < 10) {
		lcd_print_string("00");
		lcd_print_int(measured_time.milliseconds);
	} else if (measured_time.milliseconds < 100){
		lcd_print_string("0");
		lcd_print_int(measured_time.milliseconds);
	} else {
		lcd_print_int(measured_time.milliseconds);
	}
	lcd_print_string("sec");
		
}

