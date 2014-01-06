#include <xc.h>
#include <delays.h>
#include "lcd.h"


#define _LCDPORTD

#ifdef _LCDPORTD
#define E		PORTDbits.RD0
#define T_E		TRISDbits.TRISD0
#define RS		PORTDbits.RD1
#define T_RS		TRISDbits.TRISD1
#define RW		PORTDbits.RD2
#define T_RW		TRISDbits.TRISD2

#define DATA_P  	PORTD
#define TRIS_DATA_P	TRISD
#endif

#ifdef _LCDPORTC
#define E		PORTCbits.RC0
#define T_E		TRISCbits.TRISC0
#define RS		PORTCbits.RC1
#define T_RS		TRISCbits.TRISC1
#define RW		PORTCbits.RC2
#define T_RW		TRISCbits.TRISC2

#define DATA_P  	PORTC
#define TRIS_DATA_P	TRISC
#endif



//*****************************************************************************
//*****************************************************************************
void Delay_ms(int ms)
{
	 // Cycles = (1ms * Fosc) / 4 = (1ms * 8MHz) / 4 = 2000
	while(ms--)
		Delay1KTCYx(2);

}

//*****************************************************************************
//*****************************************************************************
unsigned char DisplayRead(char reg)
{
	unsigned char wert;

	TRIS_DATA_P |= 0xf0;

	RW=1;
        RS=reg;

	wert=0;

	E=1;
	E=1;
	E=1;
	wert=DATA_P & 0xf0;
	E=0;
	E=0;

	E=1;
	E=1;
	E=1;
	wert |= (DATA_P >> 4) & 0x0f;
	E=0;
	E=0;

	return wert;
}

//*****************************************************************************
//*****************************************************************************
void DisplayWrite(char reg, unsigned char d)
{
	TRIS_DATA_P &= 0x0f;
	DATA_P &= 0x0f;

	RW=0;
        RS=reg;

	E=1;
	E=1;
	DATA_P |= d & 0xf0;
	E=1;
	E=0;

	DATA_P &= 0x0f;
	Delay1TCY();

	E=1;
	E=1;
	DATA_P |= (d << 4) & 0xf0;
	E=1;
	E=0;

	Delay100TCYx(200);
}

//*****************************************************************************
//*****************************************************************************
void busy_lcd(void)
{
	unsigned char busy;

	do
	{
		busy=DisplayRead(0) & 0x80;
	} while(busy);
}

//*****************************************************************************
//*****************************************************************************
void LCD_Init(void)
{
	Delay_ms(15);

	DATA_P&= 0x0F;
	TRIS_DATA_P |= 0xf8; // Portbit 3 auf Eingang, da nicht benützt

	T_RW=0;
	T_RS=0;
	T_E=0;

	RW=0;
	RS=0;
	E=0;

	DisplayWrite(0,0x30);
	Delay_ms(5);

	DisplayWrite(0,0x30);
	Delay_ms(1);

	DisplayWrite(0,0x30);
	Delay_ms(1);

	DisplayWrite(0,0x20);
	Delay_ms(1);

	//Functionset, 4-Bit-Modus, 5*7 Punkte
	//Delay_ms(1);
	DisplayWrite(0,0x28);
	busy_lcd();

	// Display on/off
	DisplayWrite(0,0x0c);
	busy_lcd();

	// Display clear
	DisplayWrite(0,0x1);
	busy_lcd();

	// Set entry mode
	DisplayWrite(0,0x6);
	busy_lcd();
}

//*****************************************************************************
// links oben: x=1, y=1
//*****************************************************************************
void LCD_gotoxy(unsigned char x, unsigned char y)
{
	unsigned char adresse;

	switch(y)
	{
		case 1:
			adresse=0x00;
			break;
		case 2:
			adresse=0x40;
			break;
		case 3:
			adresse=0x14;
			break;
		case 4:
			adresse=0x54;
			break;
	}

	adresse += x-1;
	adresse |= 0x80;
	busy_lcd();
	DisplayWrite(0,adresse);
}

//*****************************************************************************
//*****************************************************************************
void LCD_string(char *text)
{
	while (*text != '\0')
	{
 		DisplayWrite(1,*text);
		text++;
	}
}

//*****************************************************************************
//*****************************************************************************
void LCD_string_xy(unsigned char x, unsigned char y,char *text)
{
	LCD_gotoxy(x,y);

	LCD_string(text);
}

//*****************************************************************************
//*****************************************************************************
void LCD_putc(unsigned char c)
{
	DisplayWrite(1,c);
	busy_lcd();
}

//*****************************************************************************
//*****************************************************************************
void LCD_Cursor_ON(void)
{
	DisplayWrite(0,0x0F);
}

//*****************************************************************************
//*****************************************************************************
void LCD_Cursor_OFF(void)
{
	DisplayWrite(0,0x0C);
}

//*****************************************************************************
//*****************************************************************************
void LCD_clear(void)
{
	DisplayWrite(0,0x01);
}