/*
 * timer.c
 *
 *  Created on: 17 janv. 2018
 *      Author: marie
 */
#include <msp430.h>
#include "timer.h"

timerA_state TAstate = Init;

void timer_sleep(int delay)
{
  TA0CCR0 = 2;
  TA0CTL = TASSEL_1 + MC_2;
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

void timerA_configuration(int sec)
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    CCR0 = 512*sec;                 // 2559 -> 5 sec
    TA0R = 0;
    TACTL = TASSEL_1 + ID_3 + MC_1;
}


void clock_init(void)
{
    //Clock Init
    // Use of ACLK for Low-Power purposes

    BCSCTL1 |= DIVA_3;       // ACLK/8
    BCSCTL3 |= XCAP_3;

}
