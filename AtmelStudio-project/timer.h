/*
 * timer.h
 *
 * Created: 02-Sep-19 4:38:08 PM
 *  Author: Илья
 */ 


#ifndef TIMER_H_
#define TIMER_H_


void timer0_init();
uint8_t timer0_check_flag();
void start_timer1();
void stop_timer1();
struct timer_value_struct check_timer1();


struct timer_value_struct{
	uint32_t period;
	uint32_t timer_vatue;
	};

#endif /* TIMER_H_ */