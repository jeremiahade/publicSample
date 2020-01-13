/*
 *RTC.c
 *
 * Created: 7/6/2019 11:39:40 PM
 *  Author: Jeremiah.A
 */ 

#include <atmel_start.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "RTC.h"
#include "twi.h"

//////////////////////////////////////////////

///////////////////////////////////////

uint8_t rtcAddress	=	0X68;
uint8_t rtcWriteAddress	=	0XD0;
uint8_t rtcReadAddress	=	0XD1;
uint8_t rtcControlAddress	=	0X07;
uint8_t	rtcSecondsAddress	=	0X00;
uint8_t	rtcMinutesAddress	=	0X01;
uint8_t	rtcHoursAddress	=	0X02;
uint8_t	rtcDayAddress	=	0X03;
uint8_t	rtcDateAddress	=	0X04;
uint8_t	rtcMonthAddress	=	0X05;
uint8_t	rtcYearAddress	=	0X06;	



uint8_t dec2bcd(uint8_t d)
{
	return ((d/10 * 16) + (d % 10));
}

uint8_t bcd2dec(uint8_t b)
{
	return ((b/16 * 10) + (b %  16));
}

 /* @brief  Convert a binary coded decimal value to binary. RTC stores time/date values as BCD.
    @param val BCD value
    @return Binary value
*/
/**************************************************************************/
 uint8_t bcd2bin (uint8_t val) 
 {
	  return val - 6 * (val >> 4); 
 }

/**************************************************************************/
/*!
    @brief  Convert a binary value to BCD format for the RTC registers
    @param val Binary value
    @return BCD value
*/
/**************************************************************************/
 uint8_t bin2bcd (uint8_t val) 
 { 
	 return val + 6 * (val / 10); 
 }
///////////////////////////////////////////////////////////////////////////////


void RTC_Init(void)
{
	I2C_0_init();                             // Initialize the I2c module.
	i2c_start(rtcAddress);                            // Start I2C communication
	
	i2c_write(rtcWriteAddress);        // Connect to DS1307 by sending its ID on I2c Bus
	i2c_write(rtcControlAddress);// Select the Ds1307 ControlRegister to configure Ds1307
	
	i2c_write(0x00);                        // Write 0x00 to Control register to disable SQW-Out
	
	i2c_stop();                             // Stop I2C communication after initializing DS1307
}

void RTC_SetDateTime(rtc_t *rtc)
{
	I2C_Start(rtcAddress);                          // Start I2C communication
	
	i2c_write(rtcWriteAddress);      // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(rtcSecondsAddress); // Request sec RAM address at 00H
	
	i2c_write(dec2bcd(rtc->sec));                  // Write sec from RAM address 00H
	i2c_write(dec2bcd(rtc->min));                    // Write min from RAM address 01H
	i2c_write(dec2bcd(rtc->hour) );                    // Write hour from RAM address 02H
	i2c_write(dec2bcd(rtc->weekDay));                // Write weekDay on RAM address 03H
	i2c_write(dec2bcd(rtc->date));                    // Write date on RAM address 04H
	i2c_write(dec2bcd(rtc->month));                    // Write month on RAM address 05H
	i2c_write(dec2bcd(rtc->year));                    // Write year on RAM address 06h
	
	i2c_stop();                              // Stop I2C communication after Setting the Date
}

void RTC_GetDateTime(rtc_t *rtc)
{
	I2C_Start(rtcAddress);                            // Start I2C communication
	
	i2c_write(rtcWriteAddress);        // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(rtcSecondsAddress); // Request Sec RAM address at 00H
	
	I2C_Repeated_Start(rtcReadAddress);                            // Start I2C communication
	            // connect to DS1307(Read mode) by sending its ID
	
	rtc->sec = bcd2dec(i2c_read_ack());                // read second and return Positive ACK
	rtc->min = bcd2dec(i2c_read_ack());                  // read minute and return Positive ACK
	rtc->hour= bcd2dec(i2c_read_ack());               // read hour and return Negative/No ACK
	rtc->weekDay = bcd2dec(i2c_read_ack());           // read weekDay and return Positive ACK
	rtc->date= bcd2dec(i2c_read_ack());              // read Date and return Positive ACK
	rtc->month= bcd2dec(i2c_read_ack());            // read Month and return Positive ACK
	rtc->year = bcd2dec(i2c_read_nack());             // read Year and return Negative/No ACK
	
	i2c_stop();                              // Stop I2C communication after reading the Date
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************/
/*!
    @brief  Is the DS1307 running? Check the Clock Halt bit in register 0
    @return 1 if the RTC is running, 0 if not
*/
/**************************************************************************/
uint8_t rtcisrunning(void) 
{
	 int length =1;
	uint8_t data[length];
  I2C_Start(rtcAddress);
  i2c_write(0);
  i2c_stop();

  i2c_receive(rtcAddress, data,length);
  uint8_t ss = data;
  i2c_stop();
  return !(ss>>7);
}



