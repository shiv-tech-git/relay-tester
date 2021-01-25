/*
 * Learning C.c
 *
 * Created: 18-Jul-19 3:13:09 PM
 * Author : Илья
 */ 


#include "main.h"

uint8_t render;
uint8_t stopwatch_mode;
uint8_t stopwatch_run_once;



int main(void){


		_delay_ms(100);
		//uart_init(9600);
		lcd_init();
		menu_init();
		buttons_init_hardware();
		timer0_init();
		stopwatch_init();
		
	
		
		render = 1;
		stopwatch_mode = MENU_MODE;
		stopwatch_run_once = 1;
		
		
		
		while (1){
			
			if (timer0_check_flag()){
				buttons_scanner();
			}
			
			if(stopwatch_mode == MENU_MODE){
				render |= menu_button_handler();
				
				
				
				if(render){
					render = 0;
					menu_render();
				}
			}
			
			if(stopwatch_mode == STOPWATCH_ROUTINE_MODE){
				
				if(stopwatch_run_once){
					
					stopwatch_run_once = stopwatch_error();
					if(stopwatch_run_once){
						stopwatch_run_once = 0;
						stopwatch_routine();
					}
				}
				
				if (buttons_check_short_press(UP_BUTTON)){
					stopwatch_run_once = 1;
					render = 1;
					stopwatch_mode = MENU_MODE;
				}
				
			}
		}
		
		
		
	return 0;
}