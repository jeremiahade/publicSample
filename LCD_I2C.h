/*
 * IncFile1.h
 *
 * Created: 5/19/2019 9:46:20 PM
 *  Author: Jeremiah.A
 */ 


#ifndef LCD_I2C_H_INCLUDED
#define LCD_I2C_H_INCLUDED


#ifdef __cplusplus
extern "C" {
	#endif
	
void LCD_Init (void);
void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();
void LCD_Custom_Char (unsigned char loc, unsigned char *msg);
void LCD_Char_xy (char row, char pos, unsigned char str);
void LCD_Scroll_right(void);
void LCD_Backlight(void);
void LCD_GoHome(void);
void LCD_SecondLine(void);
void LCD_DisplayInt(int number, char *buff, int NumBase);
void LCD_DisplayInt_xy(int number, char *buff, int NumBase, int x_pos, int y_pos);



	
	
	
	
	

#ifdef __cplusplus
}
#endif

#endif /* LCD_I2C.H_INCLUDED */
