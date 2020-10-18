/**************************************************************
                        UART kommandoer 
 **************************************************************/
/* 
 * File:   esp8266.h
 * Author: krs
 *
 * Created on August 29, 2019, 8:26 PM
 */
#pragma once

#ifndef MCC_H
#include "../mcc_generated_files/mcc.h"
#endif

#ifndef TCPIP_H
#include "tcpip.h"
#endif

#ifndef WIFI_H
#include "wifi.h"
#endif

#ifndef DHCP_H
#include "dhcp.h"
#endif

#ifndef DNS_H
#include "dns.h"
#endif

#ifndef ESPTOUCH_H
#include "esptouch.h"
#endif

#ifndef SNTP_H
#include "sntp.h"
#endif

#ifndef SOFTAP_H
#include "softap.h"
#endif

#ifndef SSL_H
#include "ssl.h"
#endif

#ifndef WPS_H
#include "wps.h"
#endif

#ifndef I2C_H
#include "../i2c/i2c.h"
#endif

#ifndef LCD_H
#include "../lcd/lcd.h"
#endif



#ifndef ESP8266_H
#define	ESP8266_H

/**************************************************************/

//Globals
uint8_t uartInput[128];

//
struct ESPUART {
    long    baudrate; 
    uint8_t databits;
    uint8_t stopbits;
    uint8_t parity;
    uint8_t flowcontrol;
};
struct ESPUART uart; // Ny instans af uart

struct ESPFirmWare {
    char *ATVersion;
    char *SDKVersion;
    char *BinVersion;
    uint8_t updated;
};
typedef struct ESPFirmWare espfirmware_t; // Angiver type definintion

espfirmware_t firmware; // Ny instans af firmware

struct States {
    bool ok;
    bool error;
    bool ready;
    #define esp_statesInit() do { states.ok = 0; states.error = 0; states.ready = 0; } while (0);
};
typedef struct States states_t;

states_t states;

/**************************************************************/                

void    esp_getUARTConfiguration(void);
void    esp_setUARTConfiguration(void);
void    esp_uartInitialize(void);
void    esp_restartModule(void);
void    esp_getVersion(void);
void    esp_ATTestStartup(void);
void    esp_echoOff(void);
void    esp_echoOn(void);
void    esp_autoconnect(uint8_t);
uint8_t esp_uartReceive(char *);
void    esp_upgradeFirmware(void);
void    esp_uartTransmit(char *);
bool    esp_waitForOK(void);

/* Måden som der kommunikeres til FTDI over USB */
void    esp_terminalReceive(void);
void    esp_terminalTransmit(char *);
void    esp_adjustLeds(void);

/**************************************************************/                
#endif	/* ESP8266_H */