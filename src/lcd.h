/*
 * lcd.h
 *
 *  Created on: 3 d�c. 2017
 *      Author: marie
 */

#ifndef LCD_H_
#define LCD_H_

//P2OUT
#define LCD_DB4     BIT0
#define LCD_DB5     BIT1
#define LCD_DB6     BIT2
#define LCD_DB7     BIT3
#define LCD_RS      BIT4
#define LCD_EN      BIT5

//P1OUT
#define LCD_RW      BIT5
#define LCD_BK      BIT4
#define LCD_MASK    (LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7 | LCD_RS | LCD_EN)

void LCD_init(void);
void Set_new_line(void);
void LCD_print(char* string);
void clear_screen(void);
void display_on();
void display_off();
void GPIO_init(void);
void DisplayPluvioStats(unsigned int count);

#endif /* LCD_H_ */
