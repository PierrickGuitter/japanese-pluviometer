#include <msp430.h>
#include "vl6180x.h"
#include "lcd.h"
#include "timer.h"

unsigned char btn_triggered = 0, error = 0;
unsigned int count = 0;
char* CountWrite;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

    clock_init();
    gpio_init();
    lcd_init();
    vl6180x_init();
    vl6180x_continuous_shot_range();

    __bis_SR_register(GIE);

    while (1) {
        if (btn_triggered == 1) {
            btn_triggered = 0;
            lcd_display_pluvio(count);
            timerA_configuration(5);
            count = 0;
            __bis_SR_register(LPM3_bits);
        }
        if (vl6180x_read_byte(REG_INTERRUPT_STATUS_GPIO) & 0x01) {
            error = (vl6180x_read_byte(REG_RESULT_RANGE_STATUS) & 0xF0) >> 4;
            if (error == 0 && timer_A_mode == LCD_BL_CTRL) {;
                count++;
                // timer (1 sec) for count
                timer_A_mode = RANGE_MSR_MODE;
                timerA_configuration(1);

                __bis_SR_register(LPM3_bits);
            }
            vl6180x_write_byte(REG_SYS_INT_CLEAR, 0x07);
        }
    }
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1()
{
    if (P1IFG & BIT0) {
        btn_triggered = 1;
        P1IFG &= ~(BIT0);
    }else if (P1IFG & BIT1) {
        //btn_triggered = 2;
        P1IFG &= ~(BIT1);
    }

}

