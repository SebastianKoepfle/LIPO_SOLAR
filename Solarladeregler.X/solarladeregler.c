/* 
 * File:   solarladeregler.c
 * Author: Sebastian
 *
 * Created on 18. Oktober 2013, 14:13
 */
#include <pic18f4520.h>
#include <xc.h>
#include <plib.h>
#include "lcd.h"
#include <stdio.h>

#pragma config OSC = INTIO67
#pragma config LVP = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config XINST = OFF

/*
 *
 */

#define T0RELOAD 63536

#define TASTER1         PORTBbits.RB0
#define TRIS_TASTER1    TRISBbits.TRISB0

//unsigned char taster=0;
unsigned int zustand = 1;

void Init(void);

void main(void)
{
    int result;
    char strTemp[21];
    Init();

    LCD_string_xy(1,1,"Hallo");
    LCD_Cursor_OFF();

   // OpenADC( ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH0 & ADC_CH1 & ADC_INT_OFF & ADC_REF_VDD_VSS,13);
    //SetChanADC( ADC_CH0 );

    while(1)
    {
        if (TASTER1 == 1)
        {
            zustand ++;
            if(zustand > 3)
            {
                zustand = 1;
            }

            switch(zustand)
            {
                case 1:
                    /*SetChanADC( ADC_CH0 );
                    ConvertADC(); // Start conversion
                    while( BusyADC() ); // Wait for completion
                    result = ReadADC(); // Read result

                    sprintf(strTemp,"ADC0=%4i",result);
                    LCD_string_xy(1,1,strTemp);*/
                    LCD_string_xy(1,2,"Ladestrom:");
                    LCD_Cursor_ON();                    
                    break;

                case 2:
                    LCD_string_xy(1,2,"Ausgangsstrom");
                    LCD_Cursor_ON();
                    break;

                case 3:
                    LCD_string_xy(1,2,"Ladezustand");
                    LCD_Cursor_ON();
                    break;
            }
     
        }
    }
}

void Init(void)
{
    OSCCON = 0x70;
    ADCON1= 0x0f;

    TRIS_TASTER1=1;

    LCD_Init();
    /*
    OpenTimer0(TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);
    WriteTimer0(T0RELOAD);
    GIE=1;*/
}
/*
void interrupt HIGH_ISR(void)
{
    if (TMR0IF == 1)
    {
        static unsigned int ms=0;
        static unsigned char gedr1=0;

        TMR0IF=0;
        WriteTimer0(T0RELOAD);

        ms++;
        if (ms >= 1000)
        {
            ms=0;
        }

        if ((ms % 100) == 0)
        {
            if(TASTER1 == 1 && gedr1 == 0)
            {
                gedr1=1;
                taster=1;
            }
            else if(TASTER1 == 0 && gedr1 == 1)
            {
                gedr1=0;
            }
        }
    }
}*/

