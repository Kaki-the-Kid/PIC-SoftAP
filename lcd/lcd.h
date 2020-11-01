/**************************************************************
                  LCD generelle kommandoer 
 **************************************************************/
/* 
 * File:   lcd.h
 * Author: krs
 *
 * Created on September 1, 2019, 8:49 PM
 */
#pragma once

#ifndef MCC_H
#include "../mcc_generated_files/mcc.h"
#endif

#include <stdio.h>
#include <string.h>


#ifndef LCD_H
#define	LCD_H

/**************************************************************/

const uint8_t display_addr = 0b0111100; // Addresse(7) for display 0x3C 60
const char display_init[]  = {0x00, 0x38, 0x0C, 0x06};
const char first_line[]    = {0x00, 0x80};
const char shift_line[]    = {0x00, 0xC0};
const char empty_line[]    = "                ";
char text[50]              = "";

/**************************************************************/

void lcd_displayInit(void);
void lcd_write(char *, uint8_t, uint8_t);
void lcd_outputPosXY(char *string, uint8_t posX, uint8_t posY);

/**************************************************************/
#endif	/* LCD_H */