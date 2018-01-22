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

typedef enum {
    DISABLED_MODE=0,
    ECE_MODE_ONLY=1,
    IGNORE_MODE_ONLY=2,
    IGNORE_AND_ECE_MODE=3,
    SNR_MODE_ONLY=4,
    SNR_AND_ECE_MODE=5,
    SNR_AND_IGNORE_MODE=6,
    SNR_AND_IGNORE_AND_ECE_MODE=7
} vl6180x_check_enables_mode;

/*
 *  vl6180x_init
 */
void vl6180x_init(void);

/*
 *  vl6180x_write_byte
 *      - Reg               : slave reg addr
 *      - data              : data to write
 */
void vl6180x_write_byte(unsigned short Reg, unsigned char data);

/*
 *  vl6180x_read_byte
 *      - Reg               : slave reg addr
 *      - return value      : value from slave Reg addr
 */
unsigned char vl6180x_read_byte(unsigned short Reg);

/*
 *  vl6180x_single_shot_range
 *      - return value      : range from vl6180x to object or error status
 */
int vl6180x_single_shot_range(void);

/*
 *  vl6180x_continuous_shot_range
 */
void vl6180x_continuous_shot_range(void);

/*
 *  vl6180x_history_ctrl
 *      - mode              : history buffer functionnality: 0 --> disabled, 1 --> range buffering, 2 --> ALS buffering
 */
void vl6180x_history_ctrl(char mode);

/*
 *  vl6180x_check_enables
 *      - ce_mode           : which check are enabled:
 *      DISABLED_MODE=0,
 *      ECE_MODE_ONLY=1,
 *      IGNORE_MODE_ONLY=2,
 *      IGNORE_AND_ECE_MODE=3,
 *      SNR_MODE_ONLY=4,
 *      SNR_AND_ECE_MODE=5,
 *      SNR_AND_IGNORE_MODE=6,
 *      SNR_AND_IGNORE_AND_ECE_MODE=7
 */
void vl6180x_check_enables(vl6180x_check_enables_mode ce_mode);

/*
 *  vl6180x_set_vhv_rate: repeat re-calibration rate
 */
void vl6180x_set_vhv_rate(char rate);

#endif /* VL6180X_H_ */
