#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
unsigned long ulDutyCycle=10;//Duty cycle of PWM 10%
unsigned long ulPeriod;//Period of PWM

int main(void)
{
 //Set the clock
 SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
 //Configure PWM clock to match system
 SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
 //Enable the peripherals used by this program.
 SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);//Tiva Launchpad has 2 modules (0 and 1) and
 //module 1 covers led pins
 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
 ulPeriod = SysCtlClockGet()/20000;//PWM frequency 20KHz
 //Configure PF1 pins as PWM
 GPIOPinConfigure(GPIO_PF1_M1PWM5);
 GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
 //Configure PWM Options
 PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
 PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ulPeriod);
 PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ulPeriod*ulDutyCycle/100);
 //Turn on the Output pins
 PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
 //Enable the PWM generator
 PWMGenEnable(PWM1_BASE, PWM_GEN_2);

 //Configure Timer
 SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
 TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
 uint32_t ui32Period = (SysCtlClockGet() / 10);//Period of timer
 TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
 IntEnable(INT_TIMER0A);
 TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
 IntMasterEnable();
 TimerEnable(TIMER0_BASE, TIMER_A);
 //Do nothing
 while(1)
 {
 }
 }

//Timer interrupt handler
void Timer0IntHandler(void)
{
// Clear the timer interrupt
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//Load new value for ulDutyCycle
if(ulDutyCycle>=100)
{
ulDutyCycle=0;
}
else ulDutyCycle = ulDutyCycle+10;
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,ulPeriod*ulDutyCycle/100);
}

