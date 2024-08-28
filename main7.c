// try PWM
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

int main(void)
{
    // Initialize the system
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable PWM0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    // Enable GPIO Port for PWM output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinConfigure(GPIO_PF0_M0PWM0);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);

    // Configure PWM module
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, SysCtlClockGet() * 0.1); // Set period to 0.1 seconds

    // Set pulse width (duty cycle)
    uint32_t dutyCycle = PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.2; // Duty cycle of 20%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, dutyCycle);

    // Enable PWM generator
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    // Enable PWM output
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);

    while(1)
    {
        // Infinite loop
    }
}
