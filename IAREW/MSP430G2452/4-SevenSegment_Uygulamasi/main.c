
//LaunchPad �zerindeki TXD ve RXD Jumplerlar�n� ��kar�n�z

#include "io430.h"

#define   SEGMENT0   BIT0       // P2.0
#define   SEGMENT1   BIT1       // P2.1
#define   SEGMENT2   BIT2       // P2.2
#define   SEGMENT3   BIT3       // P2.3

const unsigned char segment[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};  // Seven segment say�lar�
unsigned int sayi=0,i=0;

void segment_yaz(unsigned char, char);

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);

  P1DIR = 0xFF; //port ��k���
  P2DIR = 0x0F; //se�me pinleri
  
  P2OUT = 0x0F;
  P1OUT = segment[1];
  
  TA0CTL   = MC_1 + TASSEL_2;
  TA0CCR0  = 5000 - 1;
  TA0CCTL0 = CCIE;
  
  __bis_SR_register(GIE);
  
  while(1)
  {
    sayi++;
    
    if(sayi > 9999)            
      sayi=0;
                
    __delay_cycles(50000);
  }
}

void segment_yaz(unsigned char sayi_f,char seg)
{
  P2OUT = 0;
  P1OUT = sayi_f;
  P2OUT = seg;  
}
 
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_A0_ISR(void)
{
  i++;
  
  switch(i)
  {
  case 1 : segment_yaz(segment[sayi/1000],    SEGMENT0);      break;
  case 2 : segment_yaz(segment[sayi/100 %10], SEGMENT1);      break;
  case 3 : segment_yaz(segment[sayi/10  %10], SEGMENT2);      break;
  case 4 : segment_yaz(segment[sayi%10],      SEGMENT3); i=0; break;
  } 
}