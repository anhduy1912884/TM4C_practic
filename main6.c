#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

int main(void)
{
    // System initialization
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Wait for GPIO Port F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
    // PF1 : output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    // PF1 : HIGH


    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(15999999/3);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0 );
        SysCtlDelay(15999999/3);
    }
}
