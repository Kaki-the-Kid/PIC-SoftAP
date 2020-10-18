/************************************************************************
 * PIC SoftAP Projekt                                                   *
 ************************************************************************
 * Copyright        : © 2020 Karsten 'Kaki' Reitan Sørensen             *
 * Device           : PIC18F26K22                                       *
 * Driver Version   : 1.00-development                                  *
 ************************************************************************
 I forbindelse med løsning af opgave i Embedded  Controller skulle      *
 * vi lave en styring af ESP01 fra PIC kreds. Opgaven var, at vise en   *
 * webside med data fra PIC kredsen og datalogger, som var opbygget i   *
 * tidligere fag. Derfra kom ideen til, at lave en fuldt understøttelse *
 * af alle funktioner som ESP01 har.                                    *
 ************************************************************************/

/************************************************************************
 * mcc.h inckluderer følgende filer:                                    *
 ************************************************************************
 * #include <xc.h>                                                      *
 * #include "device_config.h"                                           *
 * #include "pin_manager.h"                                             *
 * #include <stdint.h>                                                  *
 * #include <stdbool.h>                                                 *
 * #include "interrupt_manager.h"                                       *
 * #include "tmr2.h"                                                    *
 * #include "eusart1.h"                                                 *
 * #include "eusart2.h"                                                 *
 ************************************************************************/

#pragma warning disable 520
//#pragma warning disable 362
//#pragma warning disable 520 remove never called warnings
//#pragma warning disable 1498

//#pragma warning disable 1478
//#pragma warning disable 751
//#pragma warning disable 759
//#pragma warning disable 765

#ifndef MCC_H
#include "mcc_generated_files/mcc.h"
#include <string.h>
#include <xc.h>
#endif

#ifndef ESP8266_H
#include "webserver/esp8266.h"
#endif

#ifndef I2C_H
    #include "i2c.h"
#endif

//#ifndef RTC_H
//#include "rtc.h"
//#endif


/************************************************************************
 * Main application                                                     *
 * @brief:  Hoved funktion for programmet                               *
 * @param:  none                                                         *
 ************************************************************************/
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    //INTCONbits.GIE = 1;
    INTERRUPT_PeripheralInterruptEnable();
    //INTCONbits.PEIE = 1;
    
    // Setting up Serial Connection
    EUSART1_Initialize();
    
    // Opsætter timer2, til at vise flag på ledbar
    TMR2_Initialize();
    TMR2_SetInterruptHandler(esp_adjustLeds);
    
    // Opsætter timer4, til at læse data og putte dem i eeprom
    //TMR4_Initialize();
    //TMR4_SetInterruptHandler(i2c_logData);
    
    // Opstart og konfiguration af I2C bussen til LCD og EEPROM
    i2c_init();
    lcd_displayInit();
    //rtc_getTimeAll();
    //i2c_temp_testTempSensor();
    //i2c_eeprom_initEeprom();

    // Opætning og konfiguration af ESP01 modulet
    esp_statesInit(); // Slet alle flag
    esp_serverInit(); // Slet flag for server
    //esp_getVersion();
   
    // Set tilstand for vores klienter og servere
    esp_echoOff(); // Skriv AT kommandoer: esp_echoOn() eller ej esp_echoOff()
    
    
    server.wifiMode = 3; // Senere switch
    wifi_setWifiMode(server.wifiMode);
    tcpip_setConnectionType(1);// Enable/disable multiple connections.
    tcpip_setTCPReceiveMode(1); // Passive mode, bruger UART buffer

    
    switch (server.wifiMode){
        case 1: //Station mode
            wifi_connectToAP();
            tcpip_serverSetup();
            break;
        case 2: //SoftAP Mode
            softap_SoftAPInit();
            tcpip_serverSetup();
            break;
        case 3: //SoftAP+Station mode
            wifi_connectToAP();
            softap_SoftAPInit();
            tcpip_serverSetup();
            break;
        default:
            break;
    }


    while (1) {
        esp_uartReceive(0); 
    }
}


/**
 End of File
*/