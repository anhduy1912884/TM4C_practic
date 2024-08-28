#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.h"


int checkSW(void);

int main(void)
{
    // Cau hinh tan so clock = 400/2/5
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Cau hinh su dung ngoai vi la port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Cau hinh output la port F1, F2 va F3 (F1 do, F2 duong, F3 la)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Cau hinh input la port F4 (nut nhan SW1)
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Mo cong PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;

    // Cau hinh input la port F0 (nut nhan SW2)
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    uint8_t state, light;
    light = 0;
    while (1)
    {
        state = checkSW();
        if (state)
        {
            if (light == 0x04)
            {
                light = 0;
            }
            else
                light = 0x04;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, light);
        }
    }
}

int checkSW(void)
{
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
    {
        SysCtlDelay(50000);
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
        return 0;
}
