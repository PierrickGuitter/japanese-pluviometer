/*
 * vl6180x.c
 *
 *  Created on: 12 janv. 2018
 */
#include <msp430.h>
#include "vl6180x.h"

unsigned char DataLen = 0, AddrLen = 0, TXData = 0, RXData = 0;
unsigned short RegAddr = 0;


////////////////////////////////////////////////
//  I2C Init, receive and transmit functions  //
//  Only called by vl6180x functions          //
////////////////////////////////////////////////


/*
 *  i2c_init:
 *   - Init gpios for SDA and SCL
 *   - I2C master synchronous, 7 bit addr
 *   - ACLK clock divided by 12
 *   - transmit and emit
 */
static void i2c_init(void)
{
    P1SEL |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    UCB0CTL1 = UCSSEL_2 + UCSWRST;
    UCB0BR0 = 12;
    UCB0BR1 = 0;
    UCB0I2CSA = VL6180X_ADDR;
    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0TXIE + UCB0RXIE;
}


/*
 *  i2c_transmit_data:
 *  transmit txdatalen bytes
 */
static void i2c_transmit_data(unsigned char AddrLength, unsigned char TXdatalen)
{
    AddrLen = AddrLength;
    DataLen = TXdatalen;

    while (UCB0CTL1 & UCTXSTP);                 // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;                 // transmit mode + start condition (Interrupt happening soon)
    __bis_SR_register(LPM3_bits + GIE);
}


/*
 *  i2c_receive_data:
 *  receive RXlength bytes
 */
static void i2c_receive_data(unsigned char RXlength)
{
    DataLen = RXlength;
    AddrLen = 0;

    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 &= ~UCTR ;                     // Clear I2C TX flag
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);
    UCB0CTL1 |= UCTXSTP;                    // Stop before receiving (needed)
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3 w/ interrupts
 }



////////////////////////////////////////////////
//             VL6180X functions              //
//             Public functions               //
////////////////////////////////////////////////


/*
 *  vl6180x_init:
 *   - I2C init
 *   - write first configuration (proprietary registers)
 */
void vl6180x_init(void)
{
    i2c_init();

    if (vl6180x_read_byte(REG_SYS_FRESH_OUT_OF_RST) == 0x01) {
        // Mandatory : private registers
        vl6180x_write_byte(0x0207, 0x01);
        vl6180x_write_byte(0x0208, 0x01);
        vl6180x_write_byte(0x0096, 0x00);
        vl6180x_write_byte(0x0097, 0xfd);
        vl6180x_write_byte(0x00e3, 0x00);
        vl6180x_write_byte(0x00e4, 0x04);
        vl6180x_write_byte(0x00e5, 0x02);
        vl6180x_write_byte(0x00e6, 0x01);
        vl6180x_write_byte(0x00e7, 0x03);
        vl6180x_write_byte(0x00f5, 0x02);
        vl6180x_write_byte(0x00d9, 0x05);
        vl6180x_write_byte(0x00db, 0xce);
        vl6180x_write_byte(0x00dc, 0x03);
        vl6180x_write_byte(0x00dd, 0xf8);
        vl6180x_write_byte(0x009f, 0x00);
        vl6180x_write_byte(0x00a3, 0x3c);
        vl6180x_write_byte(0x00b7, 0x00);
        vl6180x_write_byte(0x00bb, 0x3c);
        vl6180x_write_byte(0x00b2, 0x09);
        vl6180x_write_byte(0x00ca, 0x09);
        vl6180x_write_byte(0x0198, 0x01);
        vl6180x_write_byte(0x01b0, 0x17);
        vl6180x_write_byte(0x01ad, 0x00);
        vl6180x_write_byte(0x00ff, 0x05);
        vl6180x_write_byte(0x0100, 0x05);
        vl6180x_write_byte(0x0199, 0x05);
        vl6180x_write_byte(0x01a6, 0x1b);
        vl6180x_write_byte(0x01ac, 0x3e);
        vl6180x_write_byte(0x01a7, 0x1f);
        vl6180x_write_byte(0x0030, 0x00);

        vl6180x_write_byte(REG_SYS_FRESH_OUT_OF_RST,0x00);
    }

    vl6180x_write_byte(REG_SYS_GRP_PARAM_HOLD,0x01);

    vl6180x_write_byte(REG_SYS_GPI1_MODE,0x30);
    vl6180x_write_byte(REG_SYS_INT_GPIO, 0x01);
    vl6180x_history_ctrl(0);
    vl6180x_write_byte(REG_RANGE_THRESH_LOW, 0x80);  //80 mm
    vl6180x_write_byte(REG_RANGE_INTERMSR_PERIOD, 0x0F); //every 200ms
    vl6180x_write_byte(REG_RANGE_CONV_TIME,0x1D);


    vl6180x_write_byte(REG_SYS_GRP_PARAM_HOLD,0x00);


    // allow only ignore range
    vl6180x_write_byte(REG_RANGE_CHECK_ENABLES,0x02);
    vl6180x_write_byte(REG_RANGE_IGNORE_HEIGHT,0x30);
    vl6180x_write_byte(REG_RANGE_IGNORE_THRESH,0x0F);

    /*
    vl6180x_write_byte(REG_RANGE_CROSSTALK_RATE);
    vl6180x_write_byte(REG_RANGE_CROSSTALK_HEIGHT);
    vl6180x_write_byte(REG_RANGE_ECE_ESTIMATE);
    vl6180x_write_byte(REG_RANGE_P2P_OFFSET);
    vl6180x_write_byte(REG_RANGE_MAX_AMB_LVL);
    vl6180x_write_byte(REG_RANGE_CHECK_ENABLES);
    vl6180x_write_byte(REG_RANGE_VHV_RECALIBRATE);
    vl6180x_write_byte(REG_RANGE_VHV_REPEAT_RATE);*/


}


/*
 *  vl6180x_write_byte:
 *   write byte to slave reg
 */
void vl6180x_write_byte(unsigned short Reg, unsigned char data)
{
    TXData = data;
    RegAddr = Reg;

    i2c_transmit_data(2,1);
}


/*
 *  vl6180x_read_byte:
 *   read byte from slave reg
 */
unsigned char vl6180x_read_byte(unsigned short Reg)
{
    RegAddr = Reg;

    i2c_transmit_data(2,0);
    i2c_receive_data(1);

    return RXData;
}


/*
 *  vl6180x_continuous_shot_range:
 *   Begin continuous shot range
 */
void vl6180x_continuous_shot_range(void)
{
    vl6180x_write_byte(REG_RANGE_START, 0x03);
}


/*
 *  vl6180x_single_shot_range:
 *   perform a single shot range and return said range
 */
int vl6180x_single_shot_range(void)
{
    int result_range = 0;

    vl6180x_write_byte(REG_RANGE_START, 0x01);
    while (!(vl6180x_read_byte(REG_INTERRUPT_STATUS_GPIO) & 0x04)) {
        if (vl6180x_read_byte(REG_RESULT_RANGE_STATUS) != 0x00) {
            return vl6180x_read_byte(REG_RESULT_RANGE_STATUS);
        }
    }
    result_range = vl6180x_read_byte(REG_RANGE_VAL);
    vl6180x_write_byte(REG_SYS_INT_CLEAR, 0x07);

    return result_range;
}


/*
 *  vl6180x_history_ctrl:
 *      Chooses mode for history buffer: disabled(0), storing range(1), or ALS(2)
 */
void vl6180x_history_ctrl(char mode)
{
    switch (mode) {
    case 0:
        // history buffer disabled
        vl6180x_write_byte(REG_SYS_HIST_CTRL, 0x00);
        break;
    case 1:
        // history buffers range measures
        vl6180x_write_byte(REG_SYS_HIST_CTRL, 0x01);
        break;
    case 2:
        // history buffers ALS measures
        vl6180x_write_byte(REG_SYS_HIST_CTRL, 0x11);
        break;
    }
}

/*
 *      Interruption routine for I2C
 */
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    switch (AddrLen) {
        case 2:
            UCB0TXBUF = (RegAddr >> 8) & 0xFF;
            AddrLen--;
            break;
        case 1:
            UCB0TXBUF = RegAddr & 0xFF;
            AddrLen--;
            break;
        case 0:
            if (DataLen) {
                if (IFG2 & UCB0TXIFG) {
                    UCB0TXBUF = TXData;
                } else if (IFG2 & UCB0RXIFG) {
                    RXData = UCB0RXBUF;
                    __bic_SR_register_on_exit(LPM3_bits);
                }
                DataLen--;
            } else {
                if (IFG2 & UCB0TXIFG) {
                    UCB0CTL1 |= UCTXSTP;
                    IFG2 &= ~UCB0TXIFG;
                    __bic_SR_register_on_exit(LPM3_bits);
                }
            }
            break;
    }
}

