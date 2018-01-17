/*
 * lcd.c
 *
 *  Created on: 12 janv. 2018
 */
#include <msp430.h>
#include "lcd.h"
#include "timer.h"

void GPIO_init(void)
{
    // deactivate Port 3 (unused)
    P3DIR = 0xFF;
    P3OUT = 0x00;

    // Port 1. Bit0 --> LCD on/off btn
    // Port 1. Bit1 --> Vl6180X int

    P1DIR &= ~(BIT0 +BIT1);
    P1OUT |= BIT0 + BIT1;
    P1REN |= BIT0 + BIT1;

    P1IE  |= BIT0 + BIT1;
    P1IFG &= ~(BIT0 + BIT1);
    P1IES |= BIT0 + BIT1;
}


void display_off()
{
    P1OUT &= ~LCD_BK;
}

void display_on()
{
    P1OUT |= LCD_BK;
}


void request_to_read(void)
{
    P2OUT &= ~LCD_EN;
    timer_sleep(2);
    P2OUT |= LCD_EN;
    timer_sleep(2);
}

void request_to_send(void)
{
    P2OUT &= ~LCD_EN;
    timer_sleep(2);
    P2OUT |= LCD_EN;
    timer_sleep(2);
    P2OUT &= ~LCD_EN;
    timer_sleep(2);
}

void send_cmd(char Byte)
{
    P2OUT &= ~LCD_MASK;
    P2OUT = (P2OUT & 0xF0) | ((Byte >> 4) & 0x0F);
    request_to_send();
    P2OUT = (P2OUT & 0xF0) | (Byte & 0x0F);
    request_to_send();
}

void send_data(char Byte)
{
    P2OUT &= ~LCD_MASK;
    P2OUT |= LCD_RS;
    P2OUT = (P2OUT & 0xF0) | ((Byte >> 4) & 0x0F);
    request_to_send();
    P2OUT = (P2OUT & 0xF0) | (Byte & 0x0F);
    request_to_send();
}

void clear_screen(void)
{
    send_cmd(0x01);
    send_cmd(0x02);
}

void Set_new_line(void)
{
    send_cmd(0xC0);
}

void LCD_init(void)
{
    P2DIR |= LCD_MASK;
    P2OUT &= ~LCD_MASK;
    P1DIR |= LCD_RW;
    P1OUT &= ~LCD_RW;
    P1DIR |= LCD_BK;
    display_off();

    timer_sleep(1000);

    P2OUT &= ~(LCD_RS | LCD_EN);
    P2OUT = 0x02;

    request_to_send();

    //send_cmd(0x33);
    //send_cmd(0x32);
    send_cmd(0x28);
    send_cmd(0x0E);
    send_cmd(0x06);

    clear_screen();

    TAstate = LCD_Timer;
}

void LCD_print(char* string)
{
    char *c;
    c = string;

    while ((c != 0) && (*c != 0)) {
        send_data(*c);
        c++;
    }
}


void DisplayInt(unsigned int value)
{
  unsigned int tmp_value = value, ValSize = 0;
  char Byte;

  tmp_value = value;
  do {
      tmp_value /=10;
      ValSize++;
  } while (tmp_value);


  switch (ValSize) {
      case 6 :
          tmp_value = value / 100000;
          Byte = tmp_value + '0';
          send_data(Byte);
          ValSize--;
      case 5 :
          value = value - tmp_value * 100000;
          tmp_value = value /10000;
          Byte = tmp_value + '0';
          send_data(Byte);
          ValSize--;
      case 4 :
          value = value - tmp_value * 10000;
          tmp_value = value /1000;
          Byte = tmp_value + '0';
          send_data(Byte);
      case 3 :
          value = value - tmp_value * 1000;
          tmp_value = value /100;
          Byte = tmp_value + '0';
          send_data(Byte);
          ValSize--;
      case 2 :
          value = value - tmp_value * 100;
          tmp_value = value /10;
          Byte = tmp_value + '0';
          send_data(Byte);
          ValSize--;
      case 1 :
          tmp_value = value - tmp_value * 10;
          Byte = tmp_value + '0';
          send_data(Byte);
          ValSize--;
      }
}

void DisplayPluvioStats(unsigned int count)
{
    unsigned int volume = 0;

    clear_screen();
    LCD_print("pluviometrie :");
    Set_new_line();
    DisplayInt(count);
    LCD_print(" --> ");
    volume = count *130 / 55;
    DisplayInt(volume);
    LCD_print("mm");
    display_on();
}
