/*
 * main4.c
 *
 *  Created on: 7 thg 7, 2023
 *      Author: admin
 */
/*
 * main3.c
 *
 *  Created on: 6 thg 7, 2023
 *      Author: admin
 */
/*  register clock of GPIO */
#define RCGC_GPIO (*((volatile unsigned long *)0x400FE608))
/*register: Digital ENable (DEN): port C , 0x4000651C=  0x40006000 (port C start address)  +  0x51C (offset of GPIO_DEN))   */
#define GPIO_PORTC_DEN (*(volatile unsigned long *)0x4000651C)
/* register: direction (output or in put) : port C, 0x40006400 = 0x40006000 (port C start address) + 0x400 (offset of GPIO_DIR)  */
#define GPIO_PORTC_DIR  (*((volatile unsigned long*) 0x40006400 ))
/* register: data : port C , 0x40025038 =  0x40006000 (port C start address) + 0x38 (0000111000)  */
#define GPIO_PORTC_DATA  (*((volatile unsigned long*) (0x400060F8) ))

#define ENABLE_PORTC_CLOCK 0b000100  // C:  000100

#define GPIO_PORTC_PIN1_EN  0x02   //   00010
#define GPIO_PORTC_PIN2_EN  0x04
#define GPIO_PORTC_PIN3_EN  0x08
#define GPIO_PORTC_PIN4_EN  0x10
#define GPIO_PORTC_PIN5_EN  0x20
#define LED1_ON 0x02
#define LED2_ON 0x04
#define LED3_ON 0x08
#define LED4_ON 0x10
#define LED5_ON 0x20
#define delay_value 400000
void delay() ;
int main (void) {
    //set up
    RCGC_GPIO|= ENABLE_PORTC_CLOCK ;


    GPIO_PORTC_DEN |= GPIO_PORTC_PIN5_EN + GPIO_PORTC_PIN4_EN + GPIO_PORTC_PIN3_EN + GPIO_PORTC_PIN2_EN + GPIO_PORTC_PIN1_EN  ;

    //GPIO_PORTC_DIR |= GPIO_PORTC_PIN1_EN ;
    //GPIO_PORTC_DIR |= GPIO_PORTC_PIN2_EN ;
    //GPIO_PORTC_DIR |= GPIO_PORTC_PIN3_EN ;
    //GPIO_PORTC_DIR |= GPIO_PORTC_PIN4_EN ;
    //GPIO_PORTC_DIR |= GPIO_PORTC_PIN5_EN ;
    GPIO_PORTC_DIR |= GPIO_PORTC_PIN5_EN+ GPIO_PORTC_PIN4_EN + GPIO_PORTC_PIN3_EN + GPIO_PORTC_PIN2_EN + GPIO_PORTC_PIN1_EN ;

  while (1) {
    GPIO_PORTC_DATA = LED5_ON ;
    delay(delay_value) ;
    GPIO_PORTC_DATA = 0x00 ;
    delay(delay_value) ;

  }

}

void delay( unsigned long value) {
    volatile unsigned long i ;
    for (i=0 ; i < value ; i++) ;
}









