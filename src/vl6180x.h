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


//  I2C & VL6180X API
void i2c_init(void);
void vl6180x_write_byte(unsigned short Reg, unsigned char data);
unsigned char vl6180x_read_byte(unsigned short Reg);
void vl6180x_init(void);
int vl6180x_single_shot_range(void);
void vl6180x_continuous_shot_range(void);

//  Private functions
void i2c_transmit_data(unsigned char AddrLength, unsigned char TXdatalen);
void i2c_receive_data(unsigned char RXlength);


#endif /* VL6180X_H_ */
