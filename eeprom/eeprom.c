/**********************************************************//**
 * File:	eeprom.cc
 * Author:  Karsten 'Kaki' Reitan S�rensen
 * Created on September 8, 2019, 10:02 PM
 * @brief:  EEPROM styring 
 **************************************************************/

#ifndef EEPROM_H
#include "eeprom.h"
#endif


/***********************************************************//*
 * @function:	eepromTest
 * @param:		none
 * @brief:		Skriver $00-0F til adresse $0000 p� EEPROM 
 **************************************************************/
void eeprom_testData(void) {
    printf("\r\nSkriv tegn til EEPROM\r\n");

    //i2c_write_serial(eeprom_addr,0,0);
    i2c_write_serial(eeprom_addr, (char*) eeprom_test, 17); //0x00, 0x00, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    printf("Skrivning faerdig\r\n");

    i2c_write_serial(eeprom_addr, (char*) eeprom_pointer, 2); //0x00, 0x00
    printf("Laes fra EEPRROM\r\n");
    __delay_ms(20);
    
    i2c_read_serial(eeprom_addr, eeprom_buffer, 15);
    printf("Og print dem ud igen\r\n");
    
    for(uint8_t i=0; i < 15; i++) {
        printf("Vaerdi fra EEPROM i buffer: %#0.2X\r\n", eeprom_buffer[i]);
    }
}


void eeprom_logData(void) {
    // Læs sidste adresse p� EEPROM i register 0 og register 1
    i2c_write_serial(eeprom_addr, (char*) eeprom_pointer, 2); //0x00, 0x00
    i2c_read_serial(eeprom_addr, eeprom_buffer, 4);

    // Hvis register 0 er 0x0000 og register 1 er 0x0004

    // er det en start på måling
    // eller hvis register 1 er forskellig fra 4 er der sket en fejl
    // Flut adresse til register 1
    // Skriv 0 på register 0
    // Læs fra sensor
    // Skriv data på eeprom
    // Læg ny adresse på register 0
}


void eeprom_initEeprom(void) {
    // Skriv addresse reg0: 0x0000 reg1:0x0004
    i2c_write_serial(eeprom_addr, (char*) eeprom_init, 6); //0x00, 0x00, 0x00, 0x00, 0x00, 0x04
    i2c_write_serial(eeprom_addr, (char*) eeprom_pointer, 2); //0x00, 0x00
}


void eeprom_dumpLogData(void) {
    // L�s adresse i resister 0
    // Hvis adressen er forskellig fra 0x0000
    // eller er der o�tr�dt en fejl
    
    // Udl�s data udfra skabelon
    
    uint8_t  i = 0;
    uint32_t i_max = 0xFFFF;
    uint8_t  j = 0;
    char     eeprom_buffer[20];
    char     tmp[130];
    char     msg[255];
    uint32_t tmp_humid = 0;
    int32_t  tmp_temp  = 0;
    
    
    eeprom_initEeprom();
    
    // EEPROM holder den interne pointe ved reads. 4 byte * 4 linier * 105 tegn = 1680 tegn
    while ( i <= i_max ) {
        i2c_read_serial(eeprom_addr, (uint8_t*) eeprom_buffer, 8); // 4 bytes x 2 linier = 8 bytes
        
        // 2 bytes * 6 linier
        j = 0;
        while ( j < 8 ) {

            tmp_humid  = (uint32_t)(( eeprom_buffer[j++] & 0b00111111 ) << 8);
            tmp_humid +=   eeprom_buffer[j++];
            tmp_humid *= 100;
            tmp_humid /= 16382;
            
            // Sensor er gyldig mellem -25 til 85 grader
            tmp_temp  = eeprom_buffer[j++] << 8;
            tmp_temp += eeprom_buffer[j++];
            tmp_temp >>= 2;
        
            tmp_temp *= 16500;
            tmp_temp /= 16382;
            tmp_temp -= 4000;

            sprintf((char*) tmp, (const char*) eeprom_template, tmp_humid, tmp_temp ); // Lav linie fra skabelon
            strcat(msg, tmp); // l�g den til besked
        }
        
        printf( "AT+CIPSEND=0,%d\r\n", (const char*) strlen(msg) ); // Starter sending
        __delay_ms(70);
        printf(msg);
        __delay_ms(70);
    }
    
    i += j;
}


/**************************************************************/

/**
 End of File
*/
