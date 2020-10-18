/* 
 * File:   wifi.h
 * Author: krs
 *
 * Created on 4. marts 2020, 19:22
 */
#pragma once

#ifndef MCC_H
#include "mcc_generated_files/mcc.h"
#endif


#ifndef WIFI_H
#define	WIFI_H

/**************************************************************
                        WIFI kommandoer 
 **************************************************************/

// Network credentials
// AT+CWLAPOPT=1,1046
//+CWLAP:("ASUS_X00PD",-40,1,0)
//+CWLAP:("SSID-JRzcM4",-59,10,0)
//+CWLAP:("Libratone 25FDF7",-67,6,0)
//+CWLAP:("SSID-gpP9xc",-71,1,0)
//+CWLAP:("SSID-4rgfwF",-73,1,0)
//+CWLAP:("SSID-9cbL47",-79,1,0)


// 1: Station mode, 2: SoftAP mode, 3: SoftAP + Station Mode
void wifi_setWifiMode(uint8_t);
void wifi_getWifiMode(void);

void wifi_network_request(void);
bool wifi_network_lookup(const char *ssid_name);
void wifi_connectToAP(void);
void wifi_disconnectFromAP(void);
/* Måden som der kommunikeres til AP */
void wifi_SetTransmissionMode(uint8_t);

struct WIFIMode {
    uint8_t stationMode;
    uint8_t softAPMode;
    uint8_t multibleConnections;
};

typedef struct WIFIMode WIFIMode_t; // Angiver type definintion

WIFIMode_t wifi; // Ny instans af wifi

/* Opret forbindelse til Access Point*/
typedef struct item_t { char *ssid; char *password; char *encryption; } item_t;
item_t network_table[] = {
    { "SSID-JRzcM4", "frbPCwvRKq",   "WPA2 AES" },
    { "WuggaNet",    "fredagsbanan", "WPA2 AES" },
    { "ASUS_X00PD",  "0c0d8599",     "WPA2 AES" },
    { "", "", "" },
    { "", "", ""}
};

/**************************************************************/                
#endif	/* WIFI_H */

/*
 End of File
*/