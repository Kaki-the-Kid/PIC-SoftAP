/***********************************************************//*
 * File:    rtc.c
 * Author:  Karsten 'Kaki' Reitan SÃ¸rensen
 * @brief:  Realtime clock
 **************************************************************/

#ifndef RTC_DS1337_H
#include "rtc_ds1337.h"
#endif


/********************************************************************
 * Funktion som returnerer alle data fra real-time clocken, og 
 * returnerer en pointer til de hentede data. Vi er så heldige at 
 * real-time har en addresse ponter med auto increment, så det er 
 * bare at sætte adresse pointer til slave adresse 0x00 og derefter 
 * læse 0x0F bytes. Alle bytes er i BCD format, så ¤e er nemme at 
 * konvertere.
 *
 * @param rtcData	Buffer til at holde de indkomne data fra RTC
 ********************************************************************/
void rtc_getTimeAll(void) 
{
    // Hent RTC tidsdata ...
    i2c_write_serial(rtc_addr, 0, 1); //rtc register 0x00
    i2c_read_serial(rtc_addr, rtcData, 16); // 0x00 til 0x0F
    
    // ... og afkod så værdier
    
    // Register 0x00 Seconds Range: 00?59
    time.seconds = convertBCD2Bytes(rtcData[secondsAddr]);
    
    // Register 0x01 Minutes Range: 00?59
    time.minutes = convertBCD2Bytes(rtcData[minutesAddr]);
    
    // Register 0x02 Hours Range: 1?12 +AM/PM or 00?23
    time.time12_n24 = rtcData[time12_n24Addr] & bit6;
    rtcData[time12_n24Addr] &= ~bit6;
    
    if ( time.time12_n24 ) //vi arbejder med 12 hour clock, så ¶i behandler AM-PM flaget
    {
        time.timePM_nAM = rtcData[timePM_nAMAddr] & 0b00100000; // Test bit 5
        rtcData[timePM_nAMAddr] &= 0b11011111; // og fjern den
    }
    time.hours   = convertBCD2Bytes(rtcData[hoursAddr]);
    
    // Register 0x03 Day Range: 1?7
    time.day     = rtcData[dayAddr];
    
    // Register 0x04 Date Range: 01?31
    time.date    = convertBCD2Bytes(rtcData[dateAddr]);
    
    // Register 0x05 Month/ Century Range: 01?12 + Century
    time.century = rtcData[centuryAddr] & bit7;
    rtcData[centuryAddr] &= 0b01111111;
    time.month   = convertBCD2Bytes(rtcData[monthAddr]);
    
    // Register 0x06 Year Range: 00?99
    time.year    = convertBCD2Bytes(rtcData[yearAddr]);
    
    // Register 0x07 Alarm 1 Seconds Range: A1M1 + 00?59
    time.A1M1    = rtcData[alarm1SecondsAddr] & bit7;
    rtcData[alarm1SecondsAddr] &= ~bit7;
    time.alarm1Seconds = convertBCD2Bytes(rtcData[alarm1SecondsAddr]);
    
    // Register 0x08 Alarm 1 Minutes Range: A1M2 + 00?59
    time.A1M2    = rtcData[alarm1MinutesAddr] & bit7;
    rtcData[alarm1MinutesAddr] &= ~bit7;
    time.alarm1Minutes = convertBCD2Bytes(rtcData[alarm1MinutesAddr]);
    
    // Register 0x09 Alarm 1 Hours Range: A1M2 + 1?12 + AM/PM or 00?23
    time.A1M2    = rtcData[alarm1HoursAddr] & bit7;
    rtcData[alarm1HoursAddr] &= ~bit7;
    time.alarm112n24 = rtcData[alarm1HoursAddr] & bit6;
    rtcData[alarm1HoursAddr] &= ~bit6;
    if ( time.alarm112n24 ) // Vi arbejder med 12 timers tid
    {
        time.alarm1PMnAM = rtcData[alarm1HoursAddr] & 0b00100000; // Test bit 5
        rtcData[alarm1HoursAddr] &= 0b11011111; // og fjern den
    }
    time.alarm1Hour = convertBCD2Bytes(rtcData[alarm1HoursAddr]);
    
    // Register 0x0A Alarm 1 Day/Date Range: A1M4 + DYnDT + 1?7 or 01?31
    time.A1M4 = rtcData[alarm1DateAddr] & bit7; //Test bit 7
    rtcData[alarm1DateAddr] &= ~bit7; // og slet den
    time.alarm1DYnDT = rtcData[alarm1DateAddr] & bit6; //Test bit 6
    rtcData[alarm1DateAddr] &= ~bit6; // og slet den
    if(time.alarm1DYnDT) // vi kigger på ¤ay muligheden
    {
        time.alarm1Day = convertBCD2Bytes(rtcData[alarm1DateAddr]);
        time.alarm1Date = 0;
    }
    else
    {
        time.alarm1Date = convertBCD2Bytes(rtcData[alarm1DateAddr]);
        time.alarm1Day = 0;
    }
    
    // Register 0x0B Alarm 2 Minutes Range: A2M2 + 00?59
    time.A2M2 =  rtcData[alarm2MinutesAddr] & bit7; //Test bit 7
    rtcData[alarm2MinutesAddr] &= 0b01111111; // og slet den
    time.alarm2Minutes = convertBCD2Bytes(rtcData[alarm2MinutesAddr]);
    
    // Register 0x0C Alarm 2 Hours Range: A2M3 + 12n24 + 1?12 + AM/PM or 00?23
    time.A2M3 = rtcData[alarm2HoursAddr] & bit7; //Test bit 7
    rtcData[alarm2HoursAddr] &= 0b01111111; // og slet den
    time.alarm212n24 = rtcData[alarm2HoursAddr] & bit6; //Test bit 6
    rtcData[alarm2HoursAddr] &= ~bit6;
    if(time.alarm212n24) 
    {
        time.alarm2PMnAM = rtcData[alarm2HoursAddr] & 0b00100000; // Test bit 5
        rtcData[alarm1HoursAddr] &= 0b11011111; // og fjern den
    }
    time.alarm2Hours = convertBCD2Bytes(rtcData[alarm2HoursAddr]);
    
    // Register 0x0D Alarm 2 Day or Alarm 2 Date Range: A2M4, DYnDT, 1?7 or 01?31
    time.A2M4 = rtcData[alarm2DateAddr] & bit7; // 
    rtcData[alarm2DateAddr] &= 0b01111111;
    time.alarm1DYnDT = rtcData[alarm2DateAddr] & bit6; // Test bit 6
    rtcData[alarm2DateAddr] &= ~bit6; // og slet den
    if(time.alarm1DYnDT) // vi kigger på ¤ay muligheden
    {
        time.alarm2Day = convertBCD2Bytes(rtcData[alarm1DateAddr]); //Range: 1-7
        time.alarm2Date = 0; 
    }
    else
    {
        time.alarm1Date = convertBCD2Bytes(rtcData[alarm1DateAddr]); //Range: 1-31
        time.alarm1Day = 0;
    }
    
    // Register: 0x0E Function: Control
    // Bit 7:     Enable Oscillator (EOSC). 
    //            This active-low bit when set to logic 0 starts the oscillator. 
    //            When this bit is set to logic 1, the oscillator is stopped. 
    //            This bit is enabled (logic 0) when power is first applied.
    time.EOSC = rtcData[rtcControlAddr] & bit7;
    
    // Bits 4, 3: Rate Select (RS2 and RS1). These bits control the frequency of 
    //            the square-wave output when the square wave has been 
    //            enabled. The table below shows the square-wave frequencies 
    //            that can be selected with the RS bits. These bits are both set 
    //            to logic 1 (32kHz) when power is first applied.
    //            SQW/INTB Output
    //            +------+-----+-----+-----------------+------+
    //            |INTCN | RS2 | RS1 | SQW/INTB OUTPUT | A2IE |
    //            +------+-----+-----+-----------------+------+
    //            |  0   |  0  |  0  |       1Hz       |   X  |
    //            |  0   |  0  |  1  |     4.096kHz    |   X  |
    //            |  0   |  1  |  0  |     8.192kHz    |   X  |
    //            |  0   |  1  |  1  |    32.768kHz    |   X  |
    //            |  1   |  X  |  X  |       A2F       |   1  |
    //            +------+-----+-----+-----------------+------+
    time.RS2 = rtcData[rtcControlAddr] & bit4;
    time.RS1 = rtcData[rtcControlAddr] & bit3;

    // Bit 2:     Interrupt Control (INTCN). This bit controls the relationship 
    //            between the two alarms and the interrupt output pins. When the 
    //            INTCN bit is set to logic 1, a match between the timekeeping 
    //            registers and the alarm 1 registers l activates the INTA pin 
    //            (provided that the alarm is enabled) and a match between the 
    //            timekeeping registers and the alarm 2 registers activates the 
    //            SQW/INTB pin (provided that the alarm is enabled). When the 
    //            INTCN bit is set to logic 0, a square wave is output on the 
    //            SQW/INTB pin. This bit is set to logic 0 when power is first 
    //            applied.
    time.INTCN = rtcData[rtcControlAddr] & bit2;

    // Bit 1:     Alarm 2 Interrupt Enable (A2IE). When set to logic 1, this bit 
    //            permits the alarm 2 flag (A2F) bit in the status register to 
    //            assert INTA (when INTCN = 0) or to assert SQW/INTB (when INTCN 
    //            = 1). When the A2IE bit is set to logic 0, the A2F bit does not 
    //            initiate an interrupt signal. The A2IE bit is disabled (logic 
    //            0) when power is first applied. 
    time.A2IE = rtcData[rtcControlAddr] & bit1;
    
    // Bit 0:     Alarm 1 Interrupt Enable (A1IE). When set to logic 1, this bit 
    //            permits the alarm 1 flag (A1F) bit in the status register to 
    //            assert INTA. When the A1IE bit is set to logic 0, the A1F bit 
    //            does not initiate the INTA signal. The A1IE bit is disabled 
    //            (logic 0) when power is first applied.
    time.A1IE = rtcData[rtcControlAddr] & bit0;
    
    // Register: 0x0FH: Funtion: Status
    //Bit 7:    Oscillator Stop Flag (OSF). A logic 1 in this bit indicates 
    //          that the oscillator either is stopped or was stopped for some 
    //          period of time and may be used to judge the validity of the 
    //          clock and calendar data. This bit is set to logic 1 anytime that 
    //          the oscillator stops. The following are examples of conditions 
    //          that can cause the OSF bit to be set:
    //          1) The first time power is applied.
    //          2) The voltage present on VCC is insufficient to support 
    //             oscillation.
    //          3) The EOSC bit is turned off.
    //          4) External influences on the crystal (e.g., noise, leakage, 
    //             etc.).
    //          This bit remains at logic 1 until written to logic 0.
    time.OSF = rtcData[rtcStatusAddr] & bit7;
 
    //Bit 1:    Alarm 2 Flag (A2F). A logic 1 in the alarm 2 flag bit indicates 
    //          that the time matched the alarm 2 registers. This flag can be 
    //          used to generate an interrupt on either INTA or SQW/INTB 
    //          depending on the status of the INTCN bit in the control register. 
    //          If the INTCN bit is set to logic 0 and A2F is at logic 1 (and 
    //          A2IE bit is also logic 1), the INTA pin goes low. If the INTCN 
    //          bit is set to logic 1 and A2F is logic 1 (and A2IE bit is also 
    //          logic 1), the SQW/INTB pin goes low. A2F is cleared when written 
    //          to logic 0. This bit can only be written to logic 0. Attempting 
    //          to write to logic 1 leaves the value unchanged.
    time.A2F = rtcData[rtcStatusAddr] & bit1;
    
    //Bit 0:    Alarm 1 Flag (A1F). A logic 1 in the alarm 1 flag bit indicates 
    //          that the time matched the alarm 1 registers. If the A1IE bit is 
    //          also logic 1, the INTA pin goes low. A1F is cleared when written 
    //          to logic 0. This bit can only be written to logic 0. Attempting 
    //          to write to logic 1 leaves the value unchanged.
    time.A1F = rtcData[rtcStatusAddr] & bit0;
}


/**************************************************************
 * Funktion som ..
 **************************************************************/
void rtc_setTimeAll(uint8_t hours, uint8_t mins, uint8_t secs, uint8_t day, uint8_t date, uint8_t month, int16_t years) 
{
    bool century = (years>1999 || (years>=0 && years <=99) )?1:0;
    
    // SÃ¦t rtc adresse til 0x00    
    char rtc_date[] = {
        convertByte2BCD(secs),
        convertByte2BCD(mins),
        convertByte2BCD(hours),
        convertByte2BCD(day),
        convertByte2BCD(date),
        (century)?(convertByte2BCD(month) & 0x80):(convertByte2BCD(month)),
        convertByte2BCD(years)
    };
    
    i2c_write_serial(rtc_addr, rtc_date, 7);
}


/************************************************************************
 * Funktion som konverterer BCD værdi til decial byte værdi
 ************************************************************************
 * @param bcdByte:  byte som skal oversættes, med control og 
 *                  status bits afmasket
 * @return result:  resultat i decimal tal
 ************************************************************************/
uint8_t convertBCD2Bytes(uint8_t bcdByte) 
{
    uint8_t result = 0;    

    result += (bcdByte & 0x00001111);
    result += ((bcdByte & 0x11110000) << 4);
    
    return result;
}

/**************************************************************
 * Funktion som ..
 **************************************************************/
uint8_t convertByte2BCD(uint8_t byte) 
{
    uint8_t result = 0;
    
    //Eks. 12
    result  = ( byte / 10 ) << 4; // Eks. hi = 1, result = 0b0001 0000
    result += byte % 10; // Eks. lo = 2 result = 0b0001 0010
    
    return result;
}

/***************************************************************************
 * Funktion som opdaterer den lokale time værdien udfra rtc værdi
 * @return time.day:    opdateret lokal værdi fra rtc
 ***************************************************************************/
uint8_t getSeconds(void) 
{ 
    uint8_t data[1];
    
    i2c_write_serial(rtc_addr, (char) secondsAddr, 1); 
    i2c_read_serial(rtc_addr, data , 1);
    time.seconds = (uint8_t) convertBCD2Bytes(data);
    
    return time.seconds; 
}

void setSeconds(void) 
{
    char data = convertByte2BCD(time.seconds);
    char rtcRegister[] = { (char) secondsAddr, data};
    i2c_write_serial(rtc_addr, rtcRegister, 2);
}

/***************************************************************************
 * Funktion som opdaterer den lokale time værdien udfra rtc værdi
 * @return time.day:    opdateret lokal værdi fra rtc
 ***************************************************************************/
uint8_t getMinutes(void) 
{ 
    uint8_t data[1];
    
    i2c_write_serial(rtc_addr, (char) minutesAddr, 1); 
    i2c_read_serial(rtc_addr, (uint8_t) data , 1);
    time.minutes = (uint8_t) convertBCD2Bytes(data);
    
    return time.minutes; 
}

void setMinutes(void) 
{
    uint8_t data = convertByte2BCD(time.minutes);
    char rtcRegister[] = { (char) minutesAddr, data};
    i2c_write_serial(rtc_addr, rtcRegister, 2);
}

/***************************************************************************
 * Funktion som opdaterer den lokale time værdien udfra rtc værdi
 * @return time.day:    opdateret lokal værdi fra rtc
 ***************************************************************************/
uint8_t getHour(void) 
{ 
    uint8_t data[1];
    
    i2c_write_serial(rtc_addr, (char) hoursAddr, 1); 
    i2c_read_serial(rtc_addr, data , 1);
    time.hours = (uint8_t) convertBCD2Bytes(data);
    
    return time.hours; 
}

void setHour(void) 
{
    char data = convertByte2BCD(time.hours);
    char rtcRegister[] = { (char) hoursAddr, data};
    i2c_write_serial(rtc_addr, rtcRegister, 2);
}

uint8_t getAMPM(void) { return false; }
void setAMPM(void) 
{
    //time.timePM_nAM = __pm_nam;
    //uint8_t data = convertByte2BCD(__pm_nam, time.  pm_namHi, time.pm_namLo);
    //uint8_t rtcRegister[] = {time.pm_namAddr, data};
    //i2c_write_serial(rtc_addr, rtcRegister, 2);
    
    //printf("Minutes BCD: %u - %s%s\r\n", data, time.pm_namHi, time.pm_namLo);
}

/***************************************************************************
 * Funktion som opdaterer den lokale time værdien udfra rtc værdi
 * @return time.day:    opdateret lokal værdi fra rtc
 ***************************************************************************/
uint8_t getDay(void) 
{
    uint8_t data[1];
    
    i2c_write_serial(rtc_addr, (char *) dayAddr, 1); 
    i2c_read_serial(rtc_addr, data , 1);
    time.day = (uint8_t) convertBCD2Bytes(*data);
    
    return time.day; 
}

void setDay(void) 
{
    char rtcRegister[] = { (char) dayAddr, (char) convertByte2BCD(time.day) };
    i2c_write_serial(rtc_addr, rtcRegister, 2); 
}

/***************************************************************************
 * Funktion som opdaterer den lokale time værdien udfra rtc værdi
 * @return time.day:    opdateret lokal værdi fra rtc
 ***************************************************************************/
uint8_t getMonth(void) 
{
    return false; 
}
void setMonth(void) 
{
    char data = convertByte2BCD(time.month);
    char rtcRegister[] = { (char) monthAddr, data };
    i2c_write_serial(rtc_addr, rtcRegister, 2);
}

uint8_t getYear(void) { return false; }
void setYear(void) 
{
    char data = convertByte2BCD(time.year);
    char rtcRegister[] = { (char) yearAddr, data };
    i2c_write_serial(rtc_addr, rtcRegister, 2);
}

/******************************************************************************
 * Alarm indstillinger registre 0x07-0x0A 
 ******************************************************************************/
/**ALARMS
 * The DS1337 contains two time-of-day/date alarms. Alarm 1 can be set by 
 * writing to registers 07h-0Ah. Alarm 2 can be set by writing to registers 
 * 0Bh?0Dh. The alarms can be programmed (by the INTCN bit of the control 
 * register) to operate in two different modes?each alarm can drive its own 
 * separate interrupt output or both alarms can drive a common interrupt output. 
 * Bit 7 of each of the time-of-day/date alarm registers are mask bits 
 * (Table 2). When all of the mask bits for each alarm are logic 0, an alarm 
 * only occurs when the values in the timekeeping registers 00h?06h match the 
 * values stored in the time-of-day/date alarm registers. The alarms can also 
 * be programmed to repeat every second, minute, hour, day, or date. Table 3 
 * shows the possible settings. Configurations not listed in the table result 
 * in illogical operation.
 * 
 * The DY/DT bits (bit 6 of the alarm day/date registers) control whether the 
 * alarm value stored in bits 0?5 of that register reflects the day of the week 
 * or the date of the month. If DY/DT is written to logic 0, the alarm is the 
 * result of a match with date of the month. If DY/DT is written to logic 1, the 
 * alarm is the result of a match with day of the week.
 * 
 * When the RTC register values match alarm register settings, the corresponding 
 * alarm flag (A1F or A2F) bit is set to logic 1. The bit(s) will remain at a 
 * logic 1 until written to a logic 0 by the user. If the corresponding alarm 
 * interrupt enable (A1IE or A2IE) is also set to logic 1, the alarm condition 
 * activates one of the interrupt output (INTA or SQW/INTB) signals. The match 
 * is tested on the once-per-second update of the time and date registers.
 */

/************************************************************************
 * Alarm 1 indstillinger registre 0x07-0x0A 
 ************************************************************************/

/**<pre>
 * +---+-------------------+---------------------------------------------------+
 * |   | ALARM 1 REGISTER  |                    ALARM RATE                     |
 * |DY/| MASK BITS (BIT 7) |                                                   |
 * |DT +----+----+----+----+                                                   |
 * |   |A1M4|A1M3|A1M2|A1M1|                                                   |
 * +---+----+----+----+----+---------------------------------------------------+
 * | X | 1  | 1  | 1  | 1  | Alarm once per second                             |
 * | X | 1  | 1  | 1  | 0  | Alarm when seconds match                          |
 * | X | 1  | 1  | 0  | 0  | Alarm when minutes and seconds match              |
 * | X | 1  | 0  | 0  | 0  | Alarm when hours, minutes, and seconds match      |
 * | 0 | 0  | 0  | 0  | 0  | Alarm when date, hours, minutes, and seconds match|
 * | 1 | 0  | 0  | 0  | 0  | Alarm when day, hours, minutes, and seconds match |
 * +---+----+----+----+----+---------------------------------------------------+
 * </pre>
 */ 
void setAlarm1Type(bool DYnDT, uint8_t alarm1Mask) 
{
    time.A1M4 = alarm1Mask & bit3;
    time.A1M3 = alarm1Mask & bit2;
    time.A1M2 = alarm1Mask & bit1;
    time.A1M1 = alarm1Mask & bit0;

    setAlarm1A1M4();
    setAlarm1A1M2();
    setAlarm1A1M3();
    setAlarm1A1M1();
}

uint8_t getAlarm1Seconds(void) { return false; }
void setAlarm1Seconds(void) 
{
}

uint8_t getAlarm1Minutes(void) { return false; }

/*****************************************************************************
 * Funktion som sætter minutter værdi for alarm 1
 * @param: none
 * @brief: time.alarm1Minutes og time.A1M2 sættes før funktionen kaldes
 *****************************************************************************/
void setAlarm1Minutes(void) 
{
    uint8_t minsReg = time.alarm1Minutes;
    minsReg += time.A1M2<<7;
    
    uint8_t transmit[] = { 0x08, minsReg };
    i2c_write_serial(rtc_addr, (char*) transmit, 2 );
}

uint8_t getAlarm1Hours(void) { return false; }

/*****************************************************************************
 * Funktion som sætter time værdi for alarm 1
 * @param: none
 * @brief: time.alarm1Hour, time.A1M3, time.alarm112n24 og time.alarm1PMnAM
 *         sættes før funktionen kaldes
 *****************************************************************************/
void setAlarm1Hours(void) 
{
    uint8_t hoursReg;
    
    hoursReg  = convertByte2BCD(time.alarm1Hour);
    hoursReg += time.A1M2<<7;
    hoursReg += time.alarm112n24<<6;
    hoursReg += (time.alarm112n24)?time.alarm2PMnAM<<5:0; //kompenser for 12 timers tid
    
    uint8_t transmit[] = { 0x09, hoursReg };
    i2c_write_serial(rtc_addr, (char*) transmit, 2 );
}

uint8_t getAlarm1Date(void)
{
    return 0;
}

void setAlarm1Date(void) {}

uint8_t getAlarm1AMPM(void) { return false; }
void setAlarm1AMPM(void) {}

// 0x0A alarm1DateAddr
void setAlarm1A1M4(void) 
{
    uint8_t tmp = getAlarm1Date();
}

// 0x09 alarm1HoursAddr
void setAlarm1A1M3(void) 
{
    uint8_t tmp = getAlarm1Hours();
}

// 0x08 alarm1MinutesAddr
void setAlarm1A1M2(void) 
{
    uint8_t minsReg = getAlarm1Minutes(); //read from rtc
    time.alarm1Minutes = minsReg & ~bit7; // update local value
    
    setAlarm1Minutes();
}

// 0x07 alarm1SecondsAddr
void setAlarm1A1M1(void) 
{
    uint8_t secsReg = getAlarm1Seconds(); // read from rtc
    time.alarm1Seconds = secsReg & ~bit7; // update local value
    secsReg = time.alarm1Seconds + (uint8_t)( time.A1M1<<7 );
    setAlarm1Seconds();
}



/************************************************************************
 * Alarm 2 indstillinger registre 0x0B-0x0D
 ************************************************************************
 * @param DYnDT:      Vælger imellem date (0) eller day (1) indstilling 
 * @param alarm2Mask: Sætter funktionen af alarm 2 
 ************************************************************************/

/**<pre>
 * +---+-------------------+---------------------------------------------------+
 * |   | ALARM 2 REGISTER  |                    ALARM RATE                     |
 * |DY/| MASK BITS (BIT 7) |                                                   |
 * |DT +------+------+-----+                                                   |
 * |   | A2M4 | A2M3 | A2M2|                                                   |
 * +---+------+------+-----+---------------------------------------------------+
 * | X |   1  |   1  |  1  | Alarm once per minute (00 seconds of every minute)|
 * | X |   1  |   1  |  0  | Alarm when minutes match                          |
 * | X |   1  |   0  |  0  | Alarm when hours and minutes match                |
 * | 0 |   0  |   0  |  0  | Alarm when date, hours, and minutes match         |
 * | 1 |   0  |   0  |  0  | Alarm when day, hours, and minutes match          |
 * +---+------+------+-----+---------------------------------------------------+
 * </pre>
 */

void setAlarm2Type(uint8_t alarm2Mask) 
{
    time.A2M4 = alarm2Mask & bit2;
    setAlarm2A2M4();
    
    time.A2M3 = alarm2Mask & bit1;
    setAlarm2A2M3();
    
    time.A2M2 = alarm2Mask & bit0;
    setAlarm2A2M2();
}

uint8_t getAlarm2Seconds(void) { return 0; }
void setAlarm2Seconds(void) {}

uint8_t getAlarm2Minutes(void) { return 0; }
void setAlarm2Minutes(void) {}

uint8_t getAlarm2Hours(void) { return 0; }
void setAlarm2Hours(void) {}

uint8_t getAlarm2Date(void) { return 0; }

void setAlarm2Date(void) {}

bool getAlarm2AMPM(void) { return 0; }
void setAlarm2AMPM(void) {}

void setAlarm2A2M2(void) {}

void setAlarm2A2M3(void) 
{
    getAlarm2Hours();
}

void setAlarm2A2M4(void) 
{
    getAlarm2Date();
}

bool getEnableOscillator(void) { return 0;}
void setEnableOscillator(bool EOSC) {}


/**************************************************************/