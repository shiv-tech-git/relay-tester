/*
 * stopwatch.h
 *
 * Created: 02-Sep-19 3:42:56 PM
 *  Author: Илья
 */ 


#ifndef STOPWATCH_H_
#define STOPWATCH_H_

struct time_struct{
	uint16_t milliseconds;
	uint16_t microseconds;
	uint8_t seconds;
};

extern struct time_struct last_measured_time;

uint8_t stopwatch_error();
void stopwatch_routine();
void stopwatch_init();



#endif /* STOPWATCH_H_ */