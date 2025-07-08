//******************************************************************************
//  MSP430G2xx3 Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  E. Chen
//  Texas Instruments, Inc
//  May 2018
//  Built with CCS Version 8.0 and IAR Embedded Workbench Version: 7.11
//******************************************************************************
#include <msp430.h>

volatile int mode=0;
int speed[3]={10,30,50};	//呼吸燈變化速度 

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= BIT6;		//P1.6 LED連接Timer A0 
    P1SEL |= BIT6;
    P1SEL2 &= ~BIT6;

    TA0CCR0 = 999;		//頻率為 1k Hz 
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1 = 0;	//佔空比設定 
    TA0CTL = TASSEL_2 + MC_1;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
    P1IE |= BIT3;
    P1IES |= BIT3;
    P1IFG &= ~BIT3;

    __enable_interrupt();

    while(1)
    {
        int i;
        for(i=0; i<=999; i+=speed[mode])
        {
            TA0CCR1 = i;
            __delay_cycles(5000);
        }
        for(i=999; i>=0; i-=speed[mode])
        {
            TA0CCR1 = i;
            __delay_cycles(5000);
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(20000);  
    if(!(P1IN & BIT3))
    {
        mode = (mode + 1) % 3;
    }
    P1IFG &= ~BIT3;
}
