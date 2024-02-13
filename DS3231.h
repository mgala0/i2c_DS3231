/**
 * @file DS3231.h
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for DS3231 RTC clock with I2C interface
 * Set and read datetmie functions implemented at this time
 * @version 0.2
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

HAL_StatusTypeDef DS3231SetDateTime(I2C_HandleTypeDef *hi2c, struct tm rtc_timedate);
HAL_StatusTypeDef DS3231ReadDateTime(I2C_HandleTypeDef *hi2c, struct tm *rtc_timedate);
HAL_StatusTypeDef DS3231SetAlarmTime(I2C_HandleTypeDef *hi2c, struct tm rtc_timedate, uint8_t alarm_number);

#endif /* INC_DS3231_H_ */
