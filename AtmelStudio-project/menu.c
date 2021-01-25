/*
 * menu.c
 *
 * Created: 26-Jul-19 1:14:28 PM
 *  Author: Илья
 */ 


#include "menu.h"
#include <stddef.h>
#include "buttons.h"
#include "lcd1602.h"
#include "stopwatch_globals.h"
#include "stopwatch.h"

	#define ITEAM_POINTER_SYMBOL 0
	uint8_t iteam_pointer_symbol[8] = {
		0b0000,
		0b11100,
		0b01110,
		0b00111,
		0b01110,
		0b11100,
		0b00000,
		0b00000,
		};
		
	#define SELECT_POINTER_SYMBOL 1
	uint8_t select_pointer_symbol[8] = {
		0b0000,
		0b00100,
		0b01110,
		0b11111,
		0b01110,
		0b00100,
		0b0000,
		0b00000,
	};
	
	const Menu_item null_item = {NULL, NULL, NULL, NULL, NULL_FUNCTION, 0, ""};
	Menu_item *dynamic_parent = NULL;
	
	uint8_t current_string;
	
	uint8_t condition_codes[2] = {NO_CONDITON, NO_CONDITON};
	char condition_titles[2][9] = {"", ""};
	//uint8_t condition_codes[2] = {0, 3};
	//char condition_titles[2][9] = {"K1:0->1", "K2:0->1"};
	
		
	
	
	
		
					//NAME				PREVIOUS			EXT				PARENT				CHILD			FUNCTION			TYPE AND CODE						TITLE
	CREATE_MENU_ITEM(main_page,			NULL_PREVIOUS,		stopwatch,		NULL_PARENT,		start,			NULL_FUNCTION,		MAIN_PAGE,							"Main")
	
	CREATE_MENU_ITEM(stopwatch,			main_page,			NULL_NEXT,		NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		STOPWATCH,							"Stopwatch")

	CREATE_MENU_ITEM(start,				NULL_PREVIOUS,		stop,			main_page,			k1_01,			NULL_FUNCTION,		DYNAMIC_PARENT | START,				"Start")
	CREATE_MENU_ITEM(stop,				start,				NULL_NEXT,		main_page,			k1_01,			NULL_FUNCTION,		DYNAMIC_PARENT | STOP,				"Stop")
	
	CREATE_MENU_ITEM(k1_01,				NULL_PREVIOUS,		k1_01d,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K1_01 << 4,			"K1:0->1")
	CREATE_MENU_ITEM(k1_01d,			k1_01,				k1_10,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K1_01d << 4,		"K1:0->1d")
	CREATE_MENU_ITEM(k1_10,				k1_01d,				k2_01,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K1_10 << 4,			"K1:1->0")
	CREATE_MENU_ITEM(k2_01,				k1_10,				k2_01d,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K2_01 << 4,			"K2:0->1")
	CREATE_MENU_ITEM(k2_01d,			k2_01,				k2_10,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K2_01d << 4,		"K2:0->1d")
	CREATE_MENU_ITEM(k2_10,				k2_01d,				u1_01,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | K2_10 << 4,			"K2:1->0")
	CREATE_MENU_ITEM(u1_01,				k2_10,				u1_10,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | U1_01 << 4,			"U1:0->1")
	CREATE_MENU_ITEM(u1_10,				u1_01,				u2_01,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | U1_10 << 4,			"U1:1->0")
	CREATE_MENU_ITEM(u2_01,				u1_10,				u2_10,			NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | U2_01 << 4,			"U2:0->1")
	CREATE_MENU_ITEM(u2_10,				u2_01,				NULL_NEXT,		NULL_PARENT,		NULL_CHILD,		NULL_FUNCTION,		DYNAMIC_CHILD | U2_10 << 4,			"U2:1->0")
	
	




void menu_init(){
	
	stopwatch_mode = MENU_MODE;
	current_item = &main_page;
	lcd_create_symbol(iteam_pointer_symbol, ITEAM_POINTER_SYMBOL);
	lcd_create_symbol(select_pointer_symbol, SELECT_POINTER_SYMBOL);
}

void strcpy_flash(char* dest, char* crs){										//copy string from flash to RAM
	
	while ((*dest = pgm_read_byte(crs)) != '\0'){
		dest++;
		crs++;
	}
}

uint8_t go_to_next_item_if_possible(){
	if(NEXT_ITEM == &NULL_NEXT)
		return 0;
	
	GO_NEXT_ITEM
	
	return 1;
}

uint8_t go_to_previous_item_if_possible(){
	if(PREVIOUS_ITEM == &NULL_PREVIOUS)
	return 0;
	
	GO_PREVIOUS_ITEM
	
	return 1;
}

uint8_t go_to_parent_item_if_possible(){
	if(ITEM_TYPE == DYNAMIC_CHILD){
		current_item = dynamic_parent;
		return 1;
	}else if(PARENT_ITEM != &NULL_PARENT){
		GO_PARENT_ITEM
		return 1;
	}
	
	return 0;
}

uint8_t go_to_child_item_if_possible(){
	if(ITEM_TYPE == DYNAMIC_PARENT){
		dynamic_parent = current_item;
		GO_CHILD_ITEM
		return 1;
	}else if(CHILD_ITEM != &NULL_CHILD){
		GO_CHILD_ITEM
		return 1;
	}

	return 0;
}

uint8_t menu_button_handler(){
	
	uint8_t button_was_pressed = 0;
	
	//short press handlers
	if (buttons_check_short_press(UP_BUTTON)){
		if(go_to_previous_item_if_possible()){
			button_was_pressed = 1;
			current_string = 0;
		}
		
	}
	
	if (buttons_check_short_press(DOWN_BUTTON)){
		if(go_to_next_item_if_possible()){
			button_was_pressed = 1;
			current_string = 1;
		}
		
	}
	
	if (buttons_check_short_press(LEFT_BUTTON)){
		if(go_to_parent_item_if_possible()){
			button_was_pressed = 1;
		}
		
		
	}
	
	if (buttons_check_short_press(RIGHT_BUTTON)){
		if(go_to_child_item_if_possible()){
			button_was_pressed = 1;
			current_string = 0;
		}
		
	}
	
	//long press handlers
	
	if (buttons_check_long_press(RIGHT_BUTTON)){
		if(ITEM_TYPE == DYNAMIC_CHILD){																		
			uint8_t temp = ((uint8_t)pgm_read_word_near(&dynamic_parent->item_data) >> 4);					//taking number of condition (0/1 - start/stop)
			condition_codes[temp] = ITEM_CODE;																//saving selected condition
			strcpy_flash(condition_titles[temp], current_item->item_name);									//save condition title for main page
			button_was_pressed = 1;
		}
	
	}

	return button_was_pressed;
}

void print_pointer(uint8_t string){
	
	if(ITEM_TYPE == DYNAMIC_CHILD){
		if(ITEM_CODE == condition_codes[((uint8_t)pgm_read_word_near(&dynamic_parent->item_data) >> 4)]){					//if this item was selected, its item code == condition_codes[start or stop (depends of dynamic parent)], need print the pointer on this string
			lcd_set_position(string, 15);
			lcd_print_symbol(SELECT_POINTER_SYMBOL);
		}
		
		if(string == 0){
			if(NEXT_ITEM_CODE == condition_codes[(uint8_t)pgm_read_word_near(&dynamic_parent->item_data) >> 4]){			//if next item was selected, need print pointer
				lcd_set_position(1, 15);
				lcd_print_symbol(SELECT_POINTER_SYMBOL);
			}
		}
		
		if(string == 1){
			if(PREVIOUS_ITEM_CODE == condition_codes[(uint8_t)pgm_read_word_near(&dynamic_parent->item_data) >> 4]){		//if previous item was selected, need print pointer
				lcd_set_position(0, 15);
				lcd_print_symbol(SELECT_POINTER_SYMBOL);
			}
		}
	}
	
	
	
}

void render_main_page(){
	
	lcd_clear();
	lcd_home();
	
	if(condition_titles[START_CONDITION][0] != '\0')								//if start condition isn't selected yet, print "nope"
		lcd_print_string(condition_titles[START_CONDITION]);
	else
		lcd_print_string(" nope");
	
	lcd_set_position(0,8);
	
		if(condition_titles[STOP_CONDITION][0] != '\0')								//if stop condition isn't selected yet, print "nope"
			lcd_print_string(condition_titles[STOP_CONDITION]);
		else
			lcd_print_string(" nope");
	
	lcd_set_position(1,0);
	lcd_print_string("Last:");
	lcd_print_int(last_measured_time.seconds);
	lcd_print_char('.');
	lcd_print_int(last_measured_time.milliseconds);
}

void render_menu_tree(){
	
	lcd_clear();
	if(NEXT_ITEM == &NULL_NEXT)						//if we back to parent and current_string == 0 but there is no NEXT_ITEM, render current item on the second string
		current_string = 1;
	if(PREVIOUS_ITEM == &NULL_PREVIOUS)				//if we back to parent and current_string == 1 but there is no PREVIOUS_ITEM, render current item on the first string
		current_string = 0;
	
	if(current_string == 0){
		lcd_set_position(0, 0);
		lcd_print_symbol(ITEAM_POINTER_SYMBOL);
		lcd_print_string_flash(current_item->item_name);
		lcd_set_position(1, 1);
		lcd_print_string_flash(NEXT_ITEM->item_name);
	} else {
		lcd_set_position(0, 1);
		lcd_print_string_flash(PREVIOUS_ITEM->item_name);
		lcd_set_position(1, 0);
		lcd_print_symbol(ITEAM_POINTER_SYMBOL);
		lcd_print_string_flash(current_item->item_name);
	}
	
	print_pointer(current_string);
	
}



void menu_render(){											//render function for external using
	
	if(ITEM_TYPE == MAIN_PAGE){
		render_main_page();
	}else if(ITEM_TYPE == STOPWATCH){
		current_item = &main_page;
		stopwatch_mode = STOPWATCH_ROUTINE_MODE;
	}else{		
		render_menu_tree();
	}
}

