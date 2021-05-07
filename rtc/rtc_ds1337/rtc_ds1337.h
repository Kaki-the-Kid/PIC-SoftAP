/**************************************************************
 * File:    rtc.h
 * Author:  Karsten 'Kaki' Reitan Sørensen
 * Created: September 8, 2019, 6:00 PM
 * @brief:  Realtime Clock header fil
 **************************************************************/
#pragma once

#ifndef MCC_H
    #include "../../mcc_generated_files/mcc.h"
#endif

#ifndef RTC_H
    #include "../rtc.h"
#endif

#ifndef I2C
    #include "../../i2c/i2c.h"
#endif

#ifndef RTC_DS1337_H
#define	RTC_DS1337_H


/**************************************************************
                        Realtime clock
 **************************************************************/

/* 00H: |      0    | 10 Seconds (3) | Seconds (4) |                       Function: Seconds, range: 00-59
   01H: |      0    | 10 Minutes (3) | Minutes (4) |                       Function: Minutes, range: 00-59
   02H: |      0    | 12/n24 | AM/PM / 10 Hour (2) | Hour (4) |            Function: Hours, range 1-12 +AM/PM 00-23
   03H: |      0    | 0 | 0 | 0 | 0 | 0 | Day (3)) |                       Function: Day, range: 1-7
   04H: |      0    | 0 | 10 Date (2) | Date (4) |                         Function: Date, range: 01-31
   05H: |   Century | 0 | 0 | 10 Month (1) | Month (4) |                           Function: Month/ Century, range: 01-12 + Century
   06H: | 10Year(4) | Year (4) |                                           Function: Year, range: 00-99
   07H: |    A1M1   | 10 Seconds (3) | Seconds (4) |                       Function: Alarm 1 Seconds, range: 00-59
   08H: |    A1M2   | 10 Minutes (3) | Minutes (4) |                       Function: Alarm 1 Minutes, range: 00-59
   09H: |    A1M3   | 12/n24 | AM/PM / 10 Hour | Hour |                    Function: Alarm 1 Hours, range: 1-12 + AM/PM 00-23range: 10 Hour
   0AH: |    A1M4   | DY/DT | 10 Date | Day | Alarm 1 Day | 1?7 | Date |   Function: Alarm 1 Date, range: 1-31
   0BH; |    A2M2   | 10 Minutes | Minutes |                               Function: Alarm 2 Minutes, range: 00-59
   0CH; |    A2M3   | 12/24 | AM/PM / 10 Hour | Hour |                     Function: Alarm 2 Hours, range: 1-12 + AM/PM, 00-23, range: 10 Hour
   0DH: |    A2M4   | DY/nDT | 10 Date / Day | Alarm 2 Day | 1-7 | Date |  Function: Alarm 2 Date, range: 01-31
   0EH: |    EOSC   | 0 | 0 | RS2 | RS1 | INTCN | A2IE | A1IE |            Function: Control
   0FH: |    OSF    | 0 | 0 | 0 | 0 | 0 | A2F | A1F |                      Funtion: Status
 */
const uint8_t rtc_addr = 0b1101000; // 7-bit adresse på Real-time clock

//
#define secondsAddr         0x00
#define minutesAddr         0x01
#define hoursAddr           0x02
#define time12_n24Addr      0x02
#define timePM_nAMAddr      0x02
#define dayAddr             0x03
#define dateAddr            0x04
#define monthAddr           0x05
#define centuryAddr         0x05
#define yearAddr            0x06
#define alarm1SecondsAddr   0x07
#define alarm1MinutesAddr   0x08
#define alarm1HoursAddr     0x09
#define alarm1DateAddr      0x0A
#define alarm2MinutesAddr   0x0B
#define alarm2HoursAddr     0x0C
#define alarm2DateAddr      0x0D
#define rtcControlAddr      0x0E
#define rtcStatusAddr       0x0F


#define bit7    0b10000000
#define bit6    0b01000000
#define bit5    0b00100000
#define bit4    0b00010000
#define bit3    0b00001000
#define bit2    0b00000100
#define bit1    0b00000010
#define bit0    0b00100001

/**************************************************************
 * Realtime Clock function prototypes
 **************************************************************/
void    rtc_init(void);

/**************************************************************
 * Realtiome Clock function prototypes
 **************************************************************/

void    rtc_ds_1337_getTimeAll(void);
void    rtc_ds_1337_setTimeAll(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, int16_t);


/**************************************************************
 * Realtiome Clock function til indstilling af tid
 **************************************************************/

uint8_t rtc_ds_1337_getSeconds(void);
void    rtc_ds_1337_setSeconds(void);

uint8_t rtc_ds_1337_getMinutes(void);
void    rtc_ds_1337_setMinutes(void);

uint8_t rtc_ds_1337_getHour(void);
void    rtc_ds_1337_setHour(void);

uint8_t rtc_ds_1337_getAMPM(void);
void    rtc_ds_1337_setAMPM(void);

uint8_t rtc_ds_1337_getDay(void);
void    rtc_ds_1337_setDay(void);

uint8_t rtc_ds_1337_getMonth(void);
void    rtc_ds_1337_setMonth(void);

uint8_t rtc_ds_1337_getYear(void);
void    rtc_ds_1337_setYear(void);




/**************************************************************
 * Alarm 1 indstillinger registre 0x07-0x0A
 **************************************************************/

void    setAlarm1(void);

uint8_t rtc_ds_1337_getAlarm1Seconds(void);
void    rtc_ds_1337_setAlarm1Seconds(void);

uint8_t rtc_ds_1337_getAlarm1Minutes(void);
void    rtc_ds_1337_setAlarm1Minutes(void);

uint8_t rtc_ds_1337_getAlarm1Hours(void);
void    rtc_ds_1337_setAlarm1Hours(void);

uint8_t rtc_ds_1337_getAlarm1Date(void);
void    rtc_ds_1337_setAlarm1Date(void);

uint8_t rtc_ds_1337_getAlarm1AMPM(void);
void    rtc_ds_1337_setAlarm1AMPM(void);

void    rtc_ds_1337_setAlarm1A1M4(void);
void    rtc_ds_1337_setAlarm1A1M2(void);
void    rtc_ds_1337_setAlarm1A1M3(void);
void    rtc_ds_1337_setAlarm1A1M1(void);


/**************************************************************
 * Alarm 2 indstillinger registre 0x0B-0x0D
 **************************************************************/

void    rtc_ds_1337_setAlarm2Type(uint8_t);

uint8_t rtc_ds_1337_getAlarm2Seconds(void);
void    rtc_ds_1337_setAlarm2Seconds(void);

uint8_t rtc_ds_1337_getAlarm2Minutes(void);
void    rtc_ds_1337_setAlarm2Minutes(void);

uint8_t rtc_ds_1337_getAlarm2Hours(void);
void    rtc_ds_1337_setAlarm2Hours(void);

uint8_t rtc_ds_1337_getAlarm2Date(void);
void    rtc_ds_1337_setAlarm2Date(void);

bool    rtc_ds_1337_getAlarm2AMPM(void);
void    rtc_ds_1337_setAlarm2AMPM(void);

void    rtc_ds_1337_setAlarm2A2M4(void);
void    rtc_ds_1337_setAlarm2A2M3(void);
void    rtc_ds_1337_setAlarm2A2M2(void);


/**************************************************************
 * Indstilling af rtc control registre 0x0E
 **************************************************************/

bool rtc_ds_1337_getEnableOscillator(void);
void rtc_ds_1337_setEnableOscillator(bool);




/**************************************************************
 * Indstilling af rtc status registre 0x0F
 **************************************************************/


/**************************************************************/


#endif	/* RTC_DS1337_H */

