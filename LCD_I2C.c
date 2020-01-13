/*
 * CFile1.c
 *
 * Created: 5/19/2019 9:45:58 PM
 *  Author: Jeremiah.A
 */ 

#include "LCD_I2C.h"
#include <twi.h>
#include <atmel_start.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>






#define X_WADR 0x7E


uint8_t LCD_Port = 0X00;
int RS = 0;
int EN = 2;
int LED = 3;


void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);/* Sending upper nibble */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port |= (1<<EN);		/* Enable pulse */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);/* Sending lower nibble */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port |= (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_ms(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);/* Sending upper nibble */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port |= (1<<RS);  /* RS=1, data reg. */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port|= (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);  /* Sending lower nibble */
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	LCD_Port |= (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	
	i2c_start(X_WADR);
	i2c_write(LCD_Port);
	i2c_stop();
	
	_delay_ms(2);
}

void LCD_Init (void)  /* LCD Initialize function */
{
	
	
	i2c_start(X_WADR);
	i2c_write(0XFF);
	i2c_stop();
	
	_delay_ms(20);		/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x33);
	LCD_Command(0x32);	/* Send for 4 bit initialization of LCD  */
	LCD_Command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);	/* Display on cursor off */
	LCD_Command(0x06);	/* Increment cursor (shift cursor to right) */
	LCD_Command(0x01);	/* Clear display screen */
}

void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
    unsigned char i;
    if(loc<8)
    {
     LCD_Command (0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++)  /* Write 8 byte for generation of 1 character */
           LCD_Char(msg[i]);      
    }   
}

void LCD_Char_xy (char row, char pos, unsigned char str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_Char(str);		/* Call LCD string function */
}

void LCD_Scroll_right(void)
{
	LCD_Command(0X1C); // SCROLL TO RIGHT
	i2c_start(X_WADR);
	i2c_write(1<<3);
	i2c_stop();
}

void LCD_Backlight(void)
{
	i2c_start(X_WADR);
	i2c_write(1<<3);
	i2c_stop();
}

void LCD_GoHome(void)
{
	LCD_Command(0x80);
}

void LCD_SecondLine(void)
{
	LCD_Command(0xC0);
}

void LCD_DisplayInt(int number, char *buff, int NumBase)
{
	itoa(number,buff,NumBase);
	LCD_String(buff);
}

void LCD_DisplayInt_xy(int number, char *buff, int NumBase, int x_pos, int y_pos)
{
	itoa(number,buff,NumBase);
	LCD_String_xy(x_pos,y_pos,buff);
}