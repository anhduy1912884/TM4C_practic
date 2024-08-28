/*
 * main3.c
 *
 *  Created on: 6 thg 7, 2023
 *      Author: admin
 */
/*  register clock of GPIO */
#define RCGC_GPIO (*((volatile unsigned long *)0x400FE608))
/*register: Digital ENable (DEN): port F , 0x4002551C=  0x40025000 (port F start address)  +  0x51C (offset of GPIO_DEN))   */
#define GPIO_PORTF_DEN (*(volatile unsigned long *)0x4002551C)
/* register: direction (output or in put) : port F, 0x40025400 = 0x40025000 (port F start address) + 0x400 (offset of GPIO_DIR)  */
#define GPIO_PORTF_DIR  (*((volatile unsigned long*) 0x40025400 ))
/* register: data : port F , 0x40025038 =  0x40025000 (port F start address) + 0x38 (0000111000)  */
#define GPIO_PORTF_DATA  (*((volatile unsigned long*) (0x400250FC) ))

#define GPIO_PORTF_LOCK (*((volatile unsigned long *) 0x40025520))
#define GPIO_PORTF_Commit (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_PullUp (*((volatile unsigned int *)0x40025510))

#define ENABLE_PORTF_CLOCK 0x20  // 100000

#define GPIO_PORTF_PIN0_EN  0x01
#define GPIO_PORTF_PIN1_EN  0x02   //   00010
#define GPIO_PORTF_PIN2_EN  0x04
#define GPIO_PORTF_PIN3_EN  0x08
#define GPIO_PORTF_PIN4_EN  0x10
#define GPIO_PORTF_PIN5_EN  0x20

#define LED1_ON 0x02
#define LED2_ON 0x04
#define LED3_ON 0x08
#define LED4_ON 0x10
#define LED5_ON 0x20

#define delay_value 400000
void delay() ;
int main (void) {
    //set up
    RCGC_GPIO|= ENABLE_PORTF_CLOCK ;
    GPIO_PORTF_LOCK |= 0x4C4F434B ; // (*((volatile unsigned long *) 0x40025520)) |= 0x4C4F434B ;
    GPIO_PORTF_Commit  |= 0x01 ;
    GPIO_PORTF_PullUp |= GPIO_PORTF_PIN0_EN ;

    GPIO_PORTF_DEN |= GPIO_PORTF_PIN5_EN + GPIO_PORTF_PIN4_EN + GPIO_PORTF_PIN3_EN + GPIO_PORTF_PIN2_EN + GPIO_PORTF_PIN1_EN + GPIO_PORTF_PIN0_EN ;
    GPIO_PORTF_DIR = 0 ;
    GPIO_PORTF_DIR |= GPIO_PORTF_PIN5_EN + GPIO_PORTF_PIN4_EN + GPIO_PORTF_PIN3_EN + GPIO_PORTF_PIN2_EN + GPIO_PORTF_PIN1_EN ;

  while (1) {
      if(!(GPIO_PORTF_DATA & 0x01))
      {
          delay(8000) ;
          if(GPIO_PORTF_DATA & 0x01) {
            if (GPIO_PORTF_DATA & 0b01000) {
                    GPIO_PORTF_DATA = 0x00 ;   // turn off
                }
            else
                    GPIO_PORTF_DATA = LED3_ON ;   // turn on
          }


      }
  }
}

void delay( unsigned long value) {
    volatile unsigned long i ;
    for (i=0 ; i < value ; i++) ;
}





