/*
 * timer.h
 *
 *  Created on: 17 janv. 2018
 *      Author: marie
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef enum { LCD_BL_CTRL=0, RANGE_MSR_MODE=1 } timerA_state;
extern timerA_state timer_A_mode;

/*
 *  clock_init
 */
void clock_init(void);

/*
 *  timerA_configuration
 *      - sec   : number of seconds
 */
void timerA_configuration(int sec);

/*
 *  timer_sleep
 *      - delay   : iteration x 62µs
 */
void timer_sleep(int delay);

#endif /* TIMER_H_ */
