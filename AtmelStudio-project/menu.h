/*
 * menu.h
 *
 * Created: 26-Jul-19 1:14:40 PM
 *  Author: Илья
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <stddef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

//default condition
#define NO_CONDITON				255
#define START_CONDITION			0
#define STOP_CONDITION			1

//menu item types
#define MAIN_PAGE			0
#define ACTION_ITEM			1
#define DYNAMIC_PARENT		3
#define DYNAMIC_CHILD		4
#define NODE_ITEM			5
#define STOPWATCH			6
#define LAST_MEASUREMENT	7

//item codes
extern uint8_t condition_codes[2];

#define K1_01					0
#define K1_10					1
#define K1_01d					2
#define K2_01					3
#define K2_10					4
#define K2_01d					5
#define U1_01					6
#define U1_10					7
#define U2_01					8
#define U2_10					9

//condition codes
#define START					(0 << 4)
#define STOP					(1 << 4)


typedef struct PROGMEM {
	void* previous;
	void* next;
	void* parent;
	void* child;	
	void (*item_function)(uint8_t);	
	uint8_t item_data;
	char item_name[];
} Menu_item;

#define CREATE_MENU_ITEM(struct_name, previous, next, parent, child, item_function, item_data, item_name) \
	extern const Menu_item previous; \
	extern const Menu_item next; \
	extern const Menu_item parent; \
	extern const Menu_item child; \
	const Menu_item struct_name = {(void*)(&previous), (void*)(&next), (void*)(&parent), (void*)(&child), item_function, item_data , item_name};



extern const Menu_item null_item;
Menu_item* current_item;


#define NULL_FUNCTION				NULL
#define NULL_PREVIOUS				null_item
#define NULL_NEXT					null_item
#define NULL_PARENT					null_item
#define NULL_CHILD					null_item
	
#define GO_NEXT_ITEM current_item = (Menu_item*)pgm_read_word_near(&current_item->next);
#define NEXT_ITEM ((Menu_item*)pgm_read_word_near(&current_item->next))

#define GO_PREVIOUS_ITEM current_item = (Menu_item*)pgm_read_word_near(&current_item->previous);
#define PREVIOUS_ITEM ((Menu_item*)pgm_read_word_near(&current_item->previous))

#define GO_PARENT_ITEM current_item = (Menu_item*)pgm_read_word_near(&current_item->parent);
#define PARENT_ITEM ((Menu_item*)pgm_read_word_near(&current_item->parent))

#define GO_CHILD_ITEM current_item = (Menu_item*)pgm_read_word_near(&current_item->child);
#define CHILD_ITEM ((Menu_item*)pgm_read_word_near(&current_item->child))

#define ITEM_TYPE				((uint8_t)pgm_read_word_near(&current_item->item_data) & 0b1111)
#define ITEM_CODE				((uint8_t)pgm_read_word_near(&current_item->item_data) >> 4)
#define NEXT_ITEM_CODE			((uint8_t)pgm_read_word_near(&NEXT_ITEM->item_data) >> 4)
#define PREVIOUS_ITEM_CODE		((uint8_t)pgm_read_word_near(&PREVIOUS_ITEM->item_data) >> 4)
#define ITEM_NAME				((char*)pgm_read_word_near(&current_item->item_name))



void menu_init();

uint8_t go_to_previous_item_if_possible();
uint8_t go_to_next_item_if_possible();
uint8_t go_to_parent_item_if_possible();
uint8_t go_to_child_item_if_possible();

uint8_t menu_button_handler();
void menu_render();

#endif /* MENU_H_ */