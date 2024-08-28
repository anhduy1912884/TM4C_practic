/*
 * main10.c
 * i2C + LCD
 *  Created on: Oct 1, 2023
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
/**start*********************************lcd ************************************/
void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_clear_display (void);  //clear display lcd

void lcd_goto_XY (int row, int col); //set proper location on screen
/***end********************************lcd ************************************/

int main(void) {


    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    InitI2C();
    DelayMs(5) ;

    lcd_init() ;  /* lcd */

    DelayMs(10) ;
    while (1) {
        lcd_goto_XY(3, 1);
        lcd_send_string("hi all");
    }
}
/* ***************************LCD library *************************************/
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd&0xf0);
    data_l = ((cmd<<4)&0xf0);
    data_t[0] = data_u|0x0C;  //en=1, rs=0
    data_t[1] = data_u|0x08;  //en=0, rs=0
    data_t[2] = data_l|0x0C;  //en=1, rs=0
    data_t[3] = data_l|0x08;  //en=0, rs=0
   // HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
    I2CSendBytes( (uint8_t *) data_t , 4 ) ;
}

void lcd_send_data (char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data&0xf0);
    data_l = ((data<<4)&0xf0);
    data_t[0] = data_u|0x0D;  //en=1, rs=0
    data_t[1] = data_u|0x09;  //en=0, rs=0
    data_t[2] = data_l|0x0D;  //en=1, rs=0
    data_t[3] = data_l|0x09;  //en=0, rs=0
   // HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
    I2CSendBytes( (uint8_t *) data_t , 4 ) ;
}

void lcd_init (void)
{
    lcd_send_cmd (0x30);
    DelayMs(5) ;
    lcd_send_cmd (0x30);
    DelayMs(1) ;
    lcd_send_cmd (0x30);
    DelayMs(10) ;
    lcd_send_cmd (0x20);  // 4bit mode
    DelayMs(10) ;

    lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    DelayMs(1) ;
    lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
    DelayMs(1) ;
    lcd_send_cmd (0x01);  // clear display
    DelayMs(1) ;
    lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  DelayMs(1) ;
    lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
//  lcd_send_cmd (0x33); /* set 4-bits interface */
//  lcd_send_cmd (0x32);
//  HAL_Delay(50);
//  lcd_send_cmd (0x28); /* start to set LCD function */
//  HAL_Delay(50);
//  lcd_send_cmd (0x01); /* clear display */
//  HAL_Delay(50);
//  lcd_send_cmd (0x06); /* set entry mode */
//  HAL_Delay(50);
//  lcd_send_cmd (0x0c); /* set display to on */
//  HAL_Delay(50);
//  lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
//  HAL_Delay(50);
//  lcd_send_cmd (0x80);
}

void lcd_send_string (char *str)
{
    while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
    lcd_send_cmd (0x01); //clear display
}

//void lcd_goto_XY (int row, int col)
//{
//  uint8_t pos_Addr;
//  if(row == 1)
//  {
//      pos_Addr = 0x80 + row - 1 + col;
//  }
//  else
//  {
//      pos_Addr = 0x80 | (0x40 + col);
//  }
//  lcd_send_cmd(pos_Addr);
//}

void lcd_goto_XY (int row, int col)
{
    uint8_t pos_Addr;
    uint8_t addr ;
    switch (row) {
        case (1) :
         {
             addr = col - 1 ;
             break;
         }
        case (2) :
         {
             addr = 0x40 + (col -1);
             break;
         }

        case (3) :
         {
             addr = 0x14 + (col -1);
             break;
         }
        case (4) :
         {
             addr = 0x54 + (col -1);
             break;
         }
    }
    pos_Addr = 0x80|addr  ;
    lcd_send_cmd(pos_Addr);
}
