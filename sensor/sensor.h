/**************************************************************
 * File:   sensor.h
 * Author: krs
 * @brief:  Temperatur sensor header fil
 *
 * Created on 3. marts 2020, 19:43
 **************************************************************/

#ifndef MCC_H
#include "../mcc_generated_files/mcc.h"
#include <string.h>
#include <ctype.h>
#endif

#ifndef I2C_H
#include "../i2c/i2c.h"
#endif

#ifndef LCD_H
#include "../lcd/lcd.h"
#endif

#ifndef SENSOR_H
#define	SENSOR_H

const uint8_t temperature_addr = 0b0100111;      // 7-bit Addresse(0x27) fra databladet
const char    temp_string[]    = "@Temp: xx,xo     ";
const char    moist_string[]   = "@Humidity: xxx%  ";
const char    minus[]          = {0x40, 0b00101101};
const char    degree_pos[]     = {0x00, 0x8A};
const char    temp_pos[]       = {0x00, 0x84};
const char    moist_pos[]      = {0x00, 0xC6};
const char    custom_char0[]   = {0x80, 0x40, 0x40, 0b01100, 0b10010, 0b10010, 0b01100, 0b00000, 0b00000, 0b00000, 0b00000};
char          tmp_string[0x0F];

void sensor_testTempSensor(void);
void sensor_getReading(void);
void sensor_updateTemperature(int8_t temp);
void sensor_updateHumidity(uint8_t);


#endif	/* SENSOR_H */

