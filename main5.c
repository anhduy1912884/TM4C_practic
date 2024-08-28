// LED change color
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

#define LED_RED GPIO_PIN_1
#define LED_GREEN GPIO_PIN_2
#define LED_BLUE GPIO_INT_PIN_3

 int tick= LED_GREEN;
 int mySystemClock = 0;

uint8_t i = 0 ;
void main (void)
{


// Configure clock 40 MHz
SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
//PortF Enable
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//Config GPIO that connect to LED - PIN 1,2,3
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE);
//Turn off all LEDs
GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
//Infinite loop
while(1)
{
GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, tick);
//tick<<=1;
//if(tick>GPIO_PIN_3)
//tick=LED_RED;
switch (tick)
{
case LED_GREEN:
    tick = LED_BLUE;
    break;

case LED_BLUE:
    tick = LED_RED ;
    break;

case LED_RED:
    tick = LED_GREEN;
    break;
}

//Delay 1s
//SysCtlDelay(count) delay 3*count cycles
mySystemClock = SysCtlClockGet() ;
SysCtlDelay(mySystemClock/3);
i >= 100 ? i = 0 : i++ ;
}
}
