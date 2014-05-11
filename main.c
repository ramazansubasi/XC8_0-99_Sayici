/* 
 * File:   main.c
 * Author: subram
 *
 * Created on 18 Ocak 2014 Cumartesi, 13:02
 */

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

#define _XTAL_FREQ 4000000
#define DIG_1 RA1
#define DIG_2 RA0
#define ARTI  RA3
#define EKSI  RA4

const unsigned char segment[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int sayi,i;

void interrupt t0_int(void)
{
    ++i;                        // i Degeri 1 Artiriliyor
    TMR0IF=0;                   //Timer0 Interrupt Flag Sifirlaniyor
}

void Tarama (void)
{
    if(i<5)                     
        {
            DIG_2=1;
            PORTB=segment[sayi/10];
            DIG_1=0;
        }
    else
        {
            DIG_1=1;
            PORTB=segment[sayi%10];
            DIG_2=0;
        }
    if(i>10)
        i=0;
}

void Buton_oku ()
{
     if (ARTI==0)
        {
            sayi=sayi+1;
            if (sayi>99)
                sayi=0;
            while(!ARTI)
                 Tarama();
        }
     if (EKSI==0)
        {
            sayi=sayi-1;
            if (sayi<0)
                sayi=99;
            while(!EKSI)
                 Tarama();
        }
}


int main()
{
    ANSEL=0;
    CMCON=7;
    TRISA=0xFC;
    TRISB=0;
    OPTION_REG=0b00000000;      //TMR0 Kesmesi Sinyali Dahili Kaynaktan
                                //Frekans Bolme Orani 1:XXX
    INTCONbits.TMR0IE = 1;      //TMR0 Tasma Kesmesi Aktif
    ei();                       //Tum Kesmeler Aktif

    while(1)
    {
        Buton_oku();
        Tarama();
    }
}

