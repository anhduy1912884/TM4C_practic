/*
 * main11.c
 *
 *  Created on: Oct 2, 2023
 *      Author: admin
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#define I2C0_BASE           0x40020000
#define I2C_SLAVE_ADDRESS   0x27

//uint32_t receiveSize = 1 ;
uint8_t rData[1];

void DelayMs(uint32_t milliseconds) {
    uint32_t delayCycles = (SysCtlClockGet() / 1000) * milliseconds;
    SysCtlDelay(delayCycles);
}

void InitI2C(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
    I2CMasterSlaveAddrSet(I2C0_BASE, I2C_SLAVE_ADDRESS, false);
    I2CMasterEnable(I2C0_BASE);
}

void I2CSendSingleByte(uint8_t data) {
    I2CMasterDataPut(I2C0_BASE, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(I2C0_BASE));
}

void I2CSendBytes(uint8_t *data, uint32_t numBytes) {
    uint32_t i;
    for (i = 0; i < numBytes; i++) {
        I2CSendSingleByte(data[i]);
    }
}

void I2CReceiveBytes(uint8_t *rData, uint32_t numBytes) {
    uint32_t i;
    for (i = 0; i < numBytes; i++) {
        if (i == (numBytes - 1)) {
            //
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        } else {
            //
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        }
        while (I2CMasterBusy(I2C0_BASE));
        rData[i] = I2CMasterDataGet(I2C0_BASE);
    }
}


int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    InitI2C();
    DelayMs(5) ;

    //


    //
        I2CReceiveBytes(rData, 1);

    while (1) {
        I2CReceiveBytes(rData, 1);

    }
}


