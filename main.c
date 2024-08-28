#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
void color1() {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0b00100);
            // Delay for a bit
            SysCtlDelay(50000);
            // Turn off the LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0b001000);
            // Delay for a bit
            SysCtlDelay(50000);
}
void color2() {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0b0010);
            // Delay for a bit
            SysCtlDelay(50000);
            // Turn off the LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0b00100);
            // Delay for a bit
            SysCtlDelay(50000);
}
void Off_all () {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0b000);
}
int i = 0 ;
int t = 0 ;
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE , GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
   // HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;

  //  GPIODirModeSet(GPIO_PORTF_BASE, SW1|SW2, GPIO_DIR_MODE_IN);
  //  GPIOPadConfigSet(GPIO_PORTF_BASE, SW1|SW2, GPIO_STRENGTH_2MA,  GPIO_PIN_TYPE_STD_WPU);
    char n = 0 ;
    while(1)
    {

        for ( i = 0 ; i < 30 ; i++)
        {
            color1() ;
        }
        Off_all() ;
        for ( t = 0 ; t < 30 ; t++)
        {
             color2() ;
        }
        Off_all() ;

       // n = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

    } //while
}
