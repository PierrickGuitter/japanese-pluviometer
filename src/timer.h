/*
 * timer.h
 *
 *  Created on: 17 janv. 2018
 *      Author: marie
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef enum { Init=0, LCD_Timer=1, Prox_detection=2 } timerA_state;
extern timerA_state TAstate;

void clock_init(void);
void timerA_configuration(int sec);
void timer_sleep(int delay);

#endif /* TIMER_H_ */
