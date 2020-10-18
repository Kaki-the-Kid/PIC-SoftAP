/* 
 * File:   rtc.h
 * Author: karst
 *
 * Created on October 18, 2020, 4:09 PM
 */

#ifndef RTC_H
#define	RTC_H

#define rtc_address     0x00
#define rtc_type        0x00 // Brug denne til at vælge i mellem de forskellige RTC moduler
                             //    Type 0: DS1337
                             //    Type 1: Adafruit RTC
// Understøttede enheder
#if (rtc_type == 0)
    #include "rtc_ds1337/rtc_ds1337.h"
#else
    #include "rtc_adafruit/rtc_adafruit.h"
#endif

// Skabeloner som bruges hvis der ønskes at udprinte til webside
const char rtc_htmlTemplate[]   = "<h1>Server tid</h1><div id ='clock' onload='startTime()'></div>";
const char rtc_fontTemplate[]   = "<link href='https://fonts.googleapis.com/css?family=Orbitron' rel='stylesheet' type='text/css'>";
const char rtc_cssTemplate[]    = "body{background:black;}#clock{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);color:#FFFF00;font-family:Orbitron;letter-spacing:7px;font-weight:bold;font-size:10em;}";
const char rtc_scriptTemplate[] = "<script>function startTime(){var today=new Date();var h=today.getHours();var m=today.getMinutes();var s=today.getSeconds();m=checkTime(m);s=checkTime(s);document.getElementById('clock').innerHTML=h+':'+m+':'+s;var t=setTimeout(startTime,500);}function checkTime(i){if(i<10){i='0'+i};return i;}</script>";

uint8_t       _clockDataString[] = {"hh:mm:ss - dd-mm-yyyy"};
uint8_t       rtcData[16];


// Struct som indeholder indkomne data fra den valgte RTC
typedef struct TIME { // Registre i Real-time clock
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
    bool    time12_n24;
    bool    timePM_nAM;
    bool    century;

    bool    A1M1;
    bool    A1M2;
    bool    A1M3;
    bool    A1M4;
    uint8_t alarm1Seconds;
    uint8_t alarm1Minutes;
    uint8_t alarm1Hour;
    uint8_t alarm1Day;
    uint8_t alarm1Date;
    bool    alarm112n24;
    bool    alarm1PMnAM;
    bool    alarm1DYnDT;

    bool    A2M2;
    bool    A2M3;
    bool    A2M4;
    uint8_t alarm2Minutes;
    uint8_t alarm2Hours;
    uint8_t alarm2Day;
    uint8_t alarm2Date;
    bool    alarm212n24;
    bool    alarm2PMnAM;
    bool    alarm2DYnDT;

    bool    EOSC;
    bool    RS2;
    bool    RS1;
    bool    INTCN;
    bool    A2IE;
    bool    A1IE;
    bool    OSF;
    bool    A2F;
    bool    A1F;
} time_t;

time_t time;


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

