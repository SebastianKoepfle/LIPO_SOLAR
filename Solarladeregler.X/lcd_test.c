#include <p18f4520.h>
#include <stdio.h>
#include <usart.h>
#include <timers.h>
#include <delays.h>
#include "lcd.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON 
#pragma config PBADEN = OFF
#pragma config BOREN = ON
#pragma config PWRT = ON
#pragma config XINST = OFF

#define T0_RELOAD 63535
char secFlag=0;

//************************************************************
// Interrupt
//************************************************************

void HIGH_ISR();
#pragma code high_vector_section=0x8
void high_vector()
{
	_asm GOTO HIGH_ISR _endasm
}
#pragma code

#pragma interrupt HIGH_ISR
void HIGH_ISR()
{
	// Timer0
	if (INTCONbits.TMR0IF)
	{
		static int count=0;
		
		INTCONbits.TMR0IF=0;
		WriteTimer0(T0_RELOAD);
		
		if (++count >= 1000)  // 1 Sekunde
		{
			count=0;
			
			secFlag=1;
		}	
	}
}		

void main()
{
	int i=1;
	char strTemp[20]="Hallo hier";
	char s,m,h;

	s=0;
	m=0;
	h=0;
	
	OSCCON|=0x70;				// 8MHz-Takt
	OSCTUNEbits.PLLEN=0;		// 4xPLL aktivieren=1

	ADCON1=0x0f;  // alle Analogeingänge werden auf digitale I/O's geschalten

	
	OpenTimer0(TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);  

	INTCONbits.GIE=1;
	
	LCD_Init();
	
	LCD_Cursor_OFF();
	
	sprintf(strTemp,"Uhr 12345");
	LCD_string_xy(1,1,strTemp);
	sprintf(strTemp,"00:00:00");
	LCD_string_xy(4,2,strTemp);
		
	while(1)
	{
		if(secFlag)
		{
			secFlag=0;
			
			s++;
			if (s >= 60)
			{
				s=0;
				m++;
				if(m >= 60)
				{
					m=0;
					h++;
					if(h >= 24)
					{
						h=0;
					}
				}
			}
						
			sprintf(strTemp,"%02i:%02i:%02i",(int)h,(int)m,(int)s);
			
			LCD_string_xy(4,2,strTemp);
			

		}	
			
	}	
}