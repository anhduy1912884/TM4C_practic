/*
 * main16.c
 *
 *  Created on: Jun 20, 2024
 *      Author: admin
 */


/*
 * main15.c
 *
 *  Created on: Jun 18, 2024
 *      Author: admin
 */


/*
 * Chuong trinh giao tiep uart voi may tinh, gui lan luot cac ki tu cua hocarm.org len may tinh
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <driverlib/adc.h>
#include <stdarg.h>
#include <stdio.h>

#define SEQUENCE_NUMBER 1

void uartPrint(uint32_t uartBase, const char * str )
{
    while (* str != '\0' )
    {
        UARTCharPut(uartBase, *str);
        str++;
    }
  //  return 0x00 ;
}



void intToStringConvert( int num, char * buffer)
{
    char tempArray[10];

    int t = 0 ;
    int i =0  ;
    int numberOfNumer = 0 ;
    while ( num != 0 )
    {
        tempArray[t++]= num%10 + 48 ;
        num = num/10 ;
        numberOfNumer++ ;

    }

    for ( i = 0 ; i < numberOfNumer ; i++)
    {
        buffer[numberOfNumer-i] = tempArray[i];
    }
}


int main(void)
{
    uint32_t ui32ADC0Value[3];  //Khai bao mang 3 phan tu chua gia tri doc duoc tu ADC
    // Thiet lap clock he thong
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Kich hoat uart0

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Kich hoat ADC

    ADCSequenceConfigure(ADC0_BASE,SEQUENCE_NUMBER,ADC_TRIGGER_PROCESSOR,0); // cau hinh bat trigger ACD
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,0,ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,1,ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,2,ADC_CTL_CH0| ADC_CTL_IE | ADC_CTL_END); // 3 step
    ADCSequenceEnable(ADC0_BASE, SEQUENCE_NUMBER);    // kich hoat adc nhan du lieu

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // cho phep uart0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // cho phep gpioA bao gom a0 và a1 cua uart duoc hoat dong
    //Cau hinh chan PA0 va PA1 lan luot la chan RX va TX
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);  //Thieu dinh nghia pin pa1 pa0 la chan cua UART
    //Cau hinh uart 0 baud 9600
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    while(1)
    {
        ADCIntClear(ADC0_BASE, SEQUENCE_NUMBER);  //Xoa cac gia tri adc co tu truoc
        ADCProcessorTrigger(ADC0_BASE, SEQUENCE_NUMBER);  //Thuc nhien trigger adc
        while (!ADCIntStatus(ADC0_BASE,SEQUENCE_NUMBER, false))   //Neu trang thai khoi tao cua ADC ok thi thuc hien buoc tiep theo
        {}
        ADCSequenceDataGet(ADC0_BASE,SEQUENCE_NUMBER , ui32ADC0Value);    //Thuc hien lay du lieu va luu lai


        char myBuffer[10];
        myBuffer[0]=48 ;
        intToStringConvert(ui32ADC0Value[0], myBuffer);

        uartPrint(UART0_BASE, "the number is ");
        uartPrint(UART0_BASE, myBuffer);
        uartPrint(UART0_BASE, "\r\n");
#ifdef INDICATOR_ENABLE
        SysCtlDelay(SysCtlClockGet()/3);
#endif
    }

}


