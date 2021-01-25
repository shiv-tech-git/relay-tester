/*
 * timer.c
 *
 * Created: 02-Sep-19 4:37:56 PM
 *  Author: Илья
 */ 


#include "stopwatch_globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "buttons.h"

#include "uart.h"

#define PERIOD_20_MS_BOTTON_VALUE 100
#define PERIOD_10_MS_BOTTON_VALUE 178

#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED_BIT		1

volatile uint8_t timer0_flag;
volatile uint32_t timer1_period;

ISR (TIMER0_OVF_vect){
	TCNT0 = PERIOD_20_MS_BOTTON_VALUE;
	timer0_flag = 1;
}

ISR (TIMER1_COMPA_vect){
	timer1_period++;
}

void timer0_init(){
	
	TCCR0 = (1 << CS02) | (0 << CS01) | (1 << CS00);				// 1024 prescaler
	TIMSK |= (1 << TOIE0);
	TCNT0 = PERIOD_20_MS_BOTTON_VALUE;
	sei();
}


uint8_t timer0_check_flag(){
	uint8_t temp;
	
	temp = timer0_flag;
		
	if(temp){
		cli();
		timer0_flag = 0;
		sei();
		
		return 1;
	}
	
	return 0;
}



void start_timer1(){
	
	cli();
	TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);					//prescaler = 1\8
	TCCR1B |= (0 << WGM13) | (1 << WGM12);								//CTC mode
	OCR1A = 50000 - 1;													//if 1 takt is 1 microsecond, then overflow on 50k its 1/20 of second.
	TIMSK |= 1 << OCIE1A;												//enable compare interrupt
	TCNT1 = 0;															//reset timer value
	SFIOR |= 1 << PSR10;												//reset prescaler
	
	LED_DDR |= 1 << LED_BIT;
	LED_PORT |= 1 << LED_BIT;
	
	sei();
	
	timer1_period = 0;
		
}


struct timer_value_struct check_timer1(){
	struct timer_value_struct temp;
	
	cli();
	temp.timer_vatue = TCNT1;
	temp.period = timer1_period;
	sei();
	
	return temp;
}

void stop_timer1(){

	cli();
	TIMSK &= ~(1 << TOIE1);												//disable timer 1 overflow interrupt
	TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);					//no prescaler, TC1 stopped
	
	LED_DDR &= ~(1 << LED_BIT);
	LED_PORT &= ~(1 << LED_BIT);
	
	sei();

}