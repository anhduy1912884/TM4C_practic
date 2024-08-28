/* PWM */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

void Timer0IntHandler(void);

    uint8_t ui8PinData = 0x02;
    // Khai bao bien
    uint32_t ui32Period;
int main(void)
{
    // Cau hinh clock he thong 400/5/2 = 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_XTAL_20MHZ | SYSCTL_OSC_MAIN);
    //Kich hoat port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   //Cau hinh 3 LED la output, o day chi su dung led 2

    //Kich hoat timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //Cau hinh timer

    ui32Period = SysCtlClockGet()/2;     //Thiet lap gia tri cua chu ky dua tren tan so he thong
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    //Kich hoat ngat tai timer 0
    IntEnable(INT_TIMER0A);
    //dang ky ngat (bat buoc phai co ham nay moi chay den ham con Timer0IntHandler
    TimerIntRegister(TIMER0_BASE, TIMER_TIMA_TIMEOUT, &Timer0IntHandler);
    //Kich hoat ngat timer
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();  // Kich hoat ngat master (global interrupt _ ngat toan cuc)
    TimerEnable(TIMER0_BASE, TIMER_A);  //Kich hoat timer0
    while(1)
    {
    }
}

//Chuong trinh ngat
void Timer0IntHandler(void)
{
    // Xoa het cac ngat timer
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Doc trang thai hien tai cua chan PF2 (LED BLUE) sau do thuc hien chop tat led voi tan so cao
    if(ui8PinData==8) {ui8PinData=2;}
    else {ui8PinData=ui8PinData*2;}
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
}
