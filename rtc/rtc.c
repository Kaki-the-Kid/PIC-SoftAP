/* 
 * File:   rtc.c
 * Author: karst
 *
 * Created on 18. oktober 2020, 15:37
 */

#include <stdio.h>
#include <stdlib.h>



/***********************************************************//*
 * File:    rtc.c
 * Author:  Karsten 'Kaki' Reitan SÃ¸rensen
 * @brief:  Realtime clock
 **************************************************************/

#ifndef RTC_H
#include "rtc.h"
#endif


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

    if(bcdByte)
    {
        result += (bcdByte & 0x00001111);
        result += ((bcdByte & 0x11110000) << 4);
    }
    
    return result;
}

/**************************************************************
 * Funktion som ..
 **************************************************************/
uint8_t convertByte2BCD(uint8_t select_byte)
{
    uint8_t result = 0;
    
    //Eks. 12
    result  = (uint8_t)(( select_byte / 10 ) << 4); // Eks. hi = 1, result = 0b0001 0000
    result += select_byte % 10; // Eks. lo = 2 result = 0b0001 0010
    
    return result;
}