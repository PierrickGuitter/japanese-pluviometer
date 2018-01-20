/*
 * timer.c
 *
 *  Created on: 17 janv. 2018
 *      Author: marie
 */
#include <msp430.h>
#include "timer.h"

timerA_state timer_A_mode = LCD_BL_CTRL;


/*
 *  timer_sleep
 *   counts delay x 500µs
 */
void timer_sleep(int delay)
{
  TA0CCR0 = 2;
  TA0CTL = TASSEL_1 + MC_1;
  int  i;
  for (i = delay; i > 0; --i) {
    TA0R = 0;
    TA0CCTL0 = CCIE;
    while (TA0R < 2) {
      __bis_SR_register(LPM3_bits|GIE);
      __no_operation();
    }
  }
  TA0CTL = 0;
}


/*
 *  timerA_configuration
 *   counts sec x 1s
 */
void timerA_configuration(int sec)
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    CCR0 = 512*sec;                 // 2559 -> 5 sec
    TA0R = 0;
    TACTL = TASSEL_1 + ID_3 + MC_1;
}


/*
 *  clock_init
 *   Init ACLK/8 for Low-Power purposes
 */
void clock_init(void)
{
    BCSCTL1 |= DIVA_3;       // ACLK/8
    BCSCTL3 |= XCAP_3;
}


/*
 *  Timer A0 interrupt service routine
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if (timer_A_mode == LCD_BL_CTRL) {
        P1OUT &= ~BIT4;                  // Toggle LCD_BK
    } else if (timer_A_mode == RANGE_MSR_MODE) {
        timer_A_mode = LCD_BL_CTRL;
    }
    TA0CCTL0 = 0;
    __bic_SR_register_on_exit(LPM3_bits);
}
