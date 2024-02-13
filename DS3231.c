/**
 * @file DS3231.c
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for DS3231 RTC clock with I2C interface
 * Set and read datetmie functions implemented at this time
 * @version 0.2
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "i2c.h"
#include "DS3231.h"

#define I2C_READ_BIT				    0x01
#define I2C_WRITE_BIT				    0x00

#define I2C_DS3231_ADDRESS              0x68

#define DS3231_DATETIME_REG             0x00
#define DS3231_DATETIME_STRUCT_SIZE     7       //there is 7 registers sec, min, hour, day, month, year, day of week
#define DS3231_1ST_ALARM_REG            0x07
#define DS3231_1ST_ALARM_STRUCT_SIZE    4       //first alarm sec, min, hour, day/date
#define DS3231_2ND_ALARM_REG            0x0B
#define DS3231_2ND_ALARM_STRUCT_SIZE    3       //second alarm min, hour, day/date
#define DS3231_CONTROL_REG              0x0E
#define DS3231_CONTROL_STATUS_REG       0x0F
#define DS3231_TEMPERATURE_REG          0x11
#define DS3231_TEMP_READ_SIZE           2


 
HAL_StatusTypeDef DS3231SetDateTime(I2C_HandleTypeDef *hi2c, struct tm rtc_timedate)
{
	uint8_t send_buffer[DS3231_DATETIME_STRUCT_SIZE] = {0};
	if ((rtc_timedate.tm_sec >= 0) && (rtc_timedate.tm_sec <60))                    //seconds
	{
	    send_buffer[0] = ((rtc_timedate.tm_sec/10)<<4) + rtc_timedate.tm_sec%10;
	}
	if ((rtc_timedate.tm_min >= 0) && (rtc_timedate.tm_min <60))                    //minutes
	{
	    send_buffer[1] = ((rtc_timedate.tm_min/10)<<4) + rtc_timedate.tm_min%10;
	}
	if ((rtc_timedate.tm_hour >= 0) && (rtc_timedate.tm_hour <24))                  //hours
	{
	    send_buffer[2] = ((rtc_timedate.tm_hour/10)<<4) + rtc_timedate.tm_hour%10;
	}
	if ((rtc_timedate.tm_wday >= 0) && (rtc_timedate.tm_wday <7))                    //weekday
	{
	    send_buffer[3] = 1; //rtc_timedate.tm_wday + 1;
	}
	if ((rtc_timedate.tm_mday >= 0) && (rtc_timedate.tm_mday <32))                  //day of the month
	{
	    send_buffer[4] = ((rtc_timedate.tm_mday/10)<<4) + rtc_timedate.tm_mday%10;
	}
	if ((rtc_timedate.tm_mon >= 0) && (rtc_timedate.tm_mon <12))                  //day of the month
	{
	    send_buffer[5] = rtc_timedate.tm_mon+1;
	}
	if ((rtc_timedate.tm_year >= 0) && (rtc_timedate.tm_year <2000))                  //day of the month
	{
	    send_buffer[6] = (((rtc_timedate.tm_year-1900)/10)<<4) + (rtc_timedate.tm_year-1900)%10;    //year if between 1900 and 1999
	}
	if (rtc_timedate.tm_year >= 2000)                  //day of the month
	{
	    send_buffer[6] = (((rtc_timedate.tm_year - 2000)/10)<<4) + (rtc_timedate.tm_year - 2000)%10; // year if 2000 and later
	    send_buffer[5] |=0x80;                                                      //set century bit
	}

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, (I2C_DS3231_ADDRESS<<1)|I2C_WRITE_BIT, DS3231_DATETIME_REG, 1 , send_buffer, DS3231_DATETIME_STRUCT_SIZE, 100);
	return status;
}

HAL_StatusTypeDef DS3231ReadDateTime(I2C_HandleTypeDef *hi2c, struct tm *rtc_timedate)
{
	uint8_t read_buffer[DS3231_DATETIME_STRUCT_SIZE] = {0};
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, (I2C_DS3231_ADDRESS<<1)|I2C_READ_BIT, DS3231_DATETIME_REG, 1, read_buffer, DS3231_DATETIME_STRUCT_SIZE, 100);
	if (status == HAL_OK)
	{
		rtc_timedate->tm_sec =  (read_buffer[0]&0x0F) + (read_buffer[0]>>4)*10;
		rtc_timedate->tm_min =  (read_buffer[1]&0x0F) + (read_buffer[1]>>4)*10;
		rtc_timedate->tm_hour = (read_buffer[2]&0x0F) + ((read_buffer[2]&0x30)>>4)*10;
		rtc_timedate->tm_mday = (read_buffer[4]&0x0F) + ((read_buffer[4]&0x30)>>4)*10;
		rtc_timedate->tm_mon =  ((read_buffer[5]&0x0F) + ((read_buffer[5]&0x10)>>4)*10)-1;
		rtc_timedate->tm_year = (read_buffer[6]&0x0F) + ((read_buffer[6]>>4)*10) + ((read_buffer[5]>>7)*100);
	}
	return status;
}

HAL_StatusTypeDef DS3231SetAlarmTime(I2C_HandleTypeDef *hi2c, struct tm rtc_time, uint8_t alarm_number)
{
	return HAL_OK;
}
