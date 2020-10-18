/**************************************************************
                  LCD generelle kommandoer 
 **************************************************************/

#ifndef LCD_H
#include "lcd.h"
#endif

#ifndef I2C_H
#include "i2c.h"
#endif


void lcd_displayInit(void) {
    #ifdef debug
        esp_terminalTransmit("Initialiserer I2C bussen\r\n");
    #endif
    i2c_write_serial(display_addr, (char*) display_init, 10);
    i2c_write_serial(display_addr, (char*) first_line, 2);
    i2c_write_serial(display_addr, (char*) empty_line, 16);
    
    i2c_write_serial(display_addr, (char*) shift_line, 2);
    i2c_write_serial(display_addr, (char*) empty_line, 16);
}


/***********************************************************//*
 * Funktion som skriver tekst på LCD, med linie og position
 **************************************************************/

void lcd_write(char *tmpmsg, uint8_t line, uint8_t pos) {
    uint8_t posLcd[] = { 0x00, ((line == 2)?(0xC0 + pos):(0x80 + pos)) }; // Beregn position
    char    msg[30];
    strcpy(msg, empty_line);
    strcpy(msg, tmpmsg);
    
    i2c_write_serial(display_addr, (char*) posLcd, 2);   // Indstil "cursor"
    i2c_write_serial(display_addr, (char*) tmpmsg, 16 ); // og skriv besked
}

void lcd_outputPosXY(char *string, uint8_t posX, uint8_t posY) {
    
    //[TODO] Lav en funktion som h�ndterer output til LCD display
}

/**************************************************************/

/**
 End of File
*/