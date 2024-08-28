/*
 * main14.c
 *
 *  Created on: Jun 18, 2024
 *      Author: admin
 */


/*
 * Chuong trinh doc ADC (ADC0 channel 0 , pin PE3)
 */
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/sysctl.h>
#include <driverlib/adc.h>

#define SEQUENCE_NUMBER 1

int main(void)
{
    uint32_t ui32ADC0Value[3];  //Khai bao mang 3 phan tu chua gia tri doc duoc tu ADC

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //Cau hinh clock he thong

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Kich hoat ADC

    ADCSequenceConfigure(ADC0_BASE,SEQUENCE_NUMBER,ADC_TRIGGER_PROCESSOR,0); // cau hinh bat trigger ACD
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,0,ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,1,ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,SEQUENCE_NUMBER,2,ADC_CTL_CH0| ADC_CTL_IE | ADC_CTL_END); // 3 step
    ADCSequenceEnable(ADC0_BASE, SEQUENCE_NUMBER);    // kich hoat adc nhan du lieu

    while(1)
    {
        ADCIntClear(ADC0_BASE, SEQUENCE_NUMBER);  //Xoa cac gia tri adc co tu truoc
        ADCProcessorTrigger(ADC0_BASE, SEQUENCE_NUMBER);  //Thuc nhien trigger adc
        while (!ADCIntStatus(ADC0_BASE,SEQUENCE_NUMBER, false))   //Neu trang thai khoi tao cua ADC ok thi thuc hien buoc tiep theo
        {}
        ADCSequenceDataGet(ADC0_BASE,SEQUENCE_NUMBER , ui32ADC0Value);    //Thuc hien lay du lieu va luu lai
    }
}
