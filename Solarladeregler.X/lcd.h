
#ifndef _LCD_H
#define _LCD_H

void Delay_ms(int ms);
void LCD_Init(void);
void LCD_gotoxy(unsigned char x, unsigned char y);
void LCD_string(char *text);
void LCD_string_xy(unsigned char x, unsigned char y, char *text);
void LCD_putc(unsigned char c);
void LCD_Cursor_ON(void);
void LCD_Cursor_OFF(void);
void LCD_clear(void);

#endif