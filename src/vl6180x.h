/*
 * vl6180x.h
 *
 *  Created on: 17 nov. 2017
 *      Author: marie
 */

#ifndef VL6180X_H_
#define VL6180X_H_


#define VL6180X_ADDR    0x29
//definitions for Registers
#define REG_VL6180X_WHO_AM_I        0x0000
#define REG_IDENTIFICATION_TIME     0x0008  //2 bytes !!
#define REG_SYS_GPI0_MODE           0x0010
#define REG_SYS_GPI1_MODE           0x0011
#define REG_SYS_HIST_CTRL           0x0012
#define REG_SYS_INT_GPIO            0x0014
#define REG_SYS_INT_CLEAR           0x0015
#define REG_SYS_FRESH_OUT_OF_RST    0x0016
#define REG_SYS_GRP_PARAM_HOLD      0x0017  // keep param for every measurements
#define REG_RANGE_START             0x0018
#define REG_RANGE_THRESH_HIGH       0x0019
#define REG_RANGE_THRESH_LOW        0x001A
#define REG_RANGE_INTERMSR_PERIOD   0x001B
#define REG_RANGE_CONV_TIME         0x001C
#define REG_RANGE_CROSSTALK_RATE    0x001E
#define REG_RANGE_CROSSTALK_HEIGHT  0x0021
#define REG_RANGE_ECE_ESTIMATE      0x0022
#define REG_RANGE_P2P_OFFSET        0x0024
#define REG_RANGE_IGNORE_HEIGHT     0x0025
#define REG_RANGE_IGNORE_THRESH     0x0026
#define REG_RANGE_MAX_AMB_LVL       0x002C
#define REG_RANGE_CHECK_ENABLES     0x002D
#define REG_RANGE_VHV_RECALIBRATE   0x002E
#define REG_RANGE_VHV_REPEAT_RATE   0x0031
#define REG_RESULT_RANGE_STATUS     0x004D
#define REG_INTERRUPT_STATUS_GPIO   0x004F
#define REG_RANGE_VAL               0x0062


unsigned char DataLen = 0, AddrLen = 0, TXData = 0, RXData = 0;
unsigned short RegAddr = 0;


//  I2C API
void I2C_init(void);
void WriteByte(unsigned short Reg, unsigned char data);
unsigned char ReadByte(unsigned short Reg);
void VL6180X_Init(void);
int VL6180X_Single_Shot_Range(void);

//  Private functions
void TransmitData(unsigned char AddrLength, unsigned char TXdatalen);
void ReceiveData(unsigned char RXlength);



void I2C_init(void)
{
    // Configure Pins for I2C
    P1SEL |= BIT6 + BIT7;       // Assign I2C pins to USCI_B0
    P1SEL2 |= BIT6 + BIT7;       // Assign I2C pins to USCI_B0


    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    UCB0CTL1 = UCSSEL_2 + UCSWRST;              //USSEL -> clock selection
    UCB0BR0 = 12;                               //Clock divider LSB
    UCB0BR1 = 0;                                //MSB
    UCB0I2CSA = VL6180X_ADDR;
    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0TXIE + UCB0RXIE;
}


void TransmitData(unsigned char AddrLength, unsigned char TXdatalen)
{
    AddrLen = AddrLength;
    DataLen = TXdatalen;                      // Bytes Data
    while (UCB0CTL1 & UCTXSTP);                 // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;                 // transmit mode + start condition (Interrupt happening soon)
    __bis_SR_register(LPM3_bits + GIE);
}

void ReceiveData(unsigned char RXlength)
{
    DataLen = RXlength;
    AddrLen = 0;                            // done by transmit
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 &= ~UCTR ;                     // Clear I2C TX flag
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);
    UCB0CTL1 |= UCTXSTP;
    __bis_SR_register(LPM3_bits + GIE);        // Enter LPM3 w/ interrupts
 }


void WriteByte(unsigned short Reg, unsigned char data)
{
    TXData = data;
    RegAddr = Reg;
    TransmitData(2,1);
}

unsigned char ReadByte(unsigned short Reg)
{
    RegAddr = Reg;
    TransmitData(2,0);
    ReceiveData(1);

    return RXData;
}

void VL6180X_Init(void)
{
    if(ReadByte(REG_SYS_FRESH_OUT_OF_RST) == 0x01) {
        // Mandatory : private registers
        WriteByte(0x0207, 0x01);
        WriteByte(0x0208, 0x01);
        WriteByte(0x0096, 0x00);
        WriteByte(0x0097, 0xfd);
        WriteByte(0x00e3, 0x00);
        WriteByte(0x00e4, 0x04);
        WriteByte(0x00e5, 0x02);
        WriteByte(0x00e6, 0x01);
        WriteByte(0x00e7, 0x03);
        WriteByte(0x00f5, 0x02);
        WriteByte(0x00d9, 0x05);
        WriteByte(0x00db, 0xce);
        WriteByte(0x00dc, 0x03);
        WriteByte(0x00dd, 0xf8);
        WriteByte(0x009f, 0x00);
        WriteByte(0x00a3, 0x3c);
        WriteByte(0x00b7, 0x00);
        WriteByte(0x00bb, 0x3c);
        WriteByte(0x00b2, 0x09);
        WriteByte(0x00ca, 0x09);
        WriteByte(0x0198, 0x01);
        WriteByte(0x01b0, 0x17);
        WriteByte(0x01ad, 0x00);
        WriteByte(0x00ff, 0x05);
        WriteByte(0x0100, 0x05);
        WriteByte(0x0199, 0x05);
        WriteByte(0x01a6, 0x1b);
        WriteByte(0x01ac, 0x3e);
        WriteByte(0x01a7, 0x1f);
        WriteByte(0x0030, 0x00);

        WriteByte(REG_SYS_FRESH_OUT_OF_RST,0x00);
    }

    WriteByte(REG_SYS_GRP_PARAM_HOLD,0x01);

    WriteByte(REG_SYS_GPI1_MODE,0x30);
    WriteByte(REG_SYS_INT_GPIO, 0x01);
    WriteByte(REG_SYS_HIST_CTRL, 0x01);
    WriteByte(REG_RANGE_THRESH_LOW, 0x50);  //50 mm
    WriteByte(REG_RANGE_INTERMSR_PERIOD, 0x0F); //every 200ms
    WriteByte(REG_RANGE_CONV_TIME,0x1D);


    WriteByte(REG_SYS_GRP_PARAM_HOLD,0x00);


    /*
    WriteByte(REG_RANGE_CROSSTALK_RATE);
    WriteByte(REG_RANGE_CROSSTALK_HEIGHT);

    WriteByte(REG_RANGE_ECE_ESTIMATE);
    WriteByte(REG_RANGE_P2P_OFFSET);
    WriteByte(REG_RANGE_IGNORE_HEIGHT);
    WriteByte(REG_RANGE_IGNORE_THRESH);
    WriteByte(REG_RANGE_MAX_AMB_LVL);
    WriteByte(REG_RANGE_CHECK_ENABLES);
    WriteByte(REG_RANGE_VHV_RECALIBRATE);
    WriteByte(REG_RANGE_VHV_REPEAT_RATE);*/


}

void VL6180X_Continuous_Shot_Range(void)
{
    WriteByte(REG_RANGE_START, 0x03);
}


int VL6180X_Single_Shot_Range(void)
{
    int result_range = 0;

    WriteByte(REG_RANGE_START, 0x01);
    while( !(ReadByte(REG_INTERRUPT_STATUS_GPIO) & 0x04)) {
        if(ReadByte(REG_RESULT_RANGE_STATUS) != 0x00) {
            return ReadByte(REG_RESULT_RANGE_STATUS);
        }
    }
    result_range = ReadByte(REG_RANGE_VAL);
    WriteByte(REG_SYS_INT_CLEAR, 0x07);
    return result_range;
}

/*
 *      Interruption routine
 */
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    //transmitter mode only for now
    switch(AddrLen) {
        case 2:
            UCB0TXBUF = (RegAddr >> 8) & 0xFF;
            AddrLen--;
            break;
        case 1:
            UCB0TXBUF = RegAddr & 0xFF;
            AddrLen--;
            break;
        case 0:
            if(DataLen){                                                    // this part only works with single data
                if(IFG2 & UCB0TXIFG){
                    UCB0TXBUF = TXData;
                }else if(IFG2 & UCB0RXIFG) {
                    RXData = UCB0RXBUF;
                    __bic_SR_register_on_exit(LPM3_bits);
                }
                DataLen--;
            }else{
                if(IFG2 & UCB0TXIFG){
                    UCB0CTL1 |= UCTXSTP;
                    IFG2 &= ~UCB0TXIFG;
                    __bic_SR_register_on_exit(LPM3_bits);
                }
            }
            break;
    }
}


#endif /* VL6180X_H_ */
