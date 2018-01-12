#include <msp430.h>
#include "vl6180x.h"
#include "lcd.h"
/**
 * blink.c
 */

unsigned char btn_triggered = 0, error = 0;
unsigned int count = 0;
char* CountWrite;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

    // deactivate Port 3 (unused)
    P3DIR = 0xFF;
    P3OUT = 0x00;

    //Clock Init
    // Use of ACLK for Low-Power purposes
    BCSCTL1 |= DIVA_3;       // ACLK/8
    BCSCTL3 |= XCAP_3;

    GPIO_Init();
    LCD_INIT();

    I2C_init();
    //VL6180X use I2C
    VL6180X_Init();
    VL6180X_Continuous_Shot_Range();

    __bis_SR_register(GIE);

    while (1) {
        if(btn_triggered == 1) {
            DisplayPluvioStats(count);
            timerA_configuration(5);
            count = 0;
            __bis_SR_register(LPM3_bits);
            btn_triggered = 0;
        }else if( ReadByte(REG_INTERRUPT_STATUS_GPIO) & 0x01 ) {
            error = (ReadByte(REG_RESULT_RANGE_STATUS) & 0xF0) >> 4;
            if( error == 0 && timerA_state == 1) {
                count++;
                // timer (1 sec) for count
                timerA_configuration(1);
                timerA_state = 2;
                __bis_SR_register(LPM3_bits);
            }
            WriteByte(REG_SYS_INT_CLEAR, 0x07);
            btn_triggered = 0;
        }
    }

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1()
{
    if(P1IFG & BIT0){
        btn_triggered = 1;
        P1IFG &= ~(BIT0);
    }else if(P1IFG & BIT1) {
        //btn_triggered = 2;
        P1IFG &= ~(BIT1);
    }

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if(timerA_state == 1) {
        P1OUT &= ~BIT4;                  // Toggle LCD_BK
    }else if(timerA_state == 2) {
        timerA_state = 1;
    }
    TA0CCTL0 = 0;
    __bic_SR_register_on_exit(LPM3_bits);
}