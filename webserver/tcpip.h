/* 
 * File:   tcpip.h
 * Author: krs
 *
 * Created on September 1, 2019, 12:14 AM
 */

#ifndef MCC_H
#include "mcc_generated_files/mcc.h"
#include <string.h>
#include <xc.h>
#endif

#ifndef TCPIP_H
#define	TCPIP_H

/**************************************************************
                TCP/IP Server - single connection 
 **************************************************************/
struct SERVER {
    bool    busy;                // Er ESP modulet optaget? Bruges ved kommandoer som tager lang tid
    uint8_t wifiMode;            // Hvad for en mode kører vi i? Station, SoftAP eller begge
    bool    multibleConnections; //CIMUX skal være 1 for at vi kan starte en server
    uint8_t maxConnections;      // Hvad er de maksimale antale forbindelser <links>
    char*   ssid[20];            // ssid for stærkeste signal
    char*   password[20];        // password for stærkeste signal
    char*   encryption[20];      // krytering for stærkeste signal
    bool    wificonnected;       // Er enheden forbundet til AP (Station)
    bool    wifigotip;           // og har vi fået IP fra enheden
    bool    internet;            // Kan vi pinge? Har vi adgang til internettet?
    char    remote_ip[20];       // Hvad er vores Station IP
    char    local_ip[15];        // Hvad er vores SoftAP IP
    uint8_t running;             // Er serveren oppe at køre?
    bool    clientConnected;     // Er der en klient? 0,CONNECTED
    bool    clientAvailable;     // ... og har vi fået en forespørgsel
    #define esp_serverInit() do { strcpy((char*)server.ssid, ""); server.wifiMode = 1; server.multibleConnections = 1; server.maxConnections = 1; server.wificonnected = 0; server.wifigotip = 0; server.internet = 0; server.running = 0;} while (0);
};

typedef struct SERVER server_t; // Angiver type difinition

server_t server; // Ny instans af tcpip server


/* Prototypes wifi Station
 * Er funktioner som forbinder til et eksisterende netværk. 
 * Kun wifi forbindelsen.
 */
void    tcpip_serverSetup(void);
uint8_t tcpip_getConnectionType(uint8_t *);
void    tcpip_setConnectionType(uint8_t);
bool    tcpip_setMaxConnections(uint8_t conn);

bool    tcpip_serverTCPIPBegin(uint16_t port);
void    tcpip_serverTCPIPStop(void);

void    tcpip_getDataLength(void);
void    tcpip_closeTCPIPMode(void);
void    tcpip_closeTCPIPConnection(void);
void    tcpip_setTCPReceiveMode(uint8_t);
void    tcpip_PassiveReceive(void);
void    tcpip_showIPDRemoteIP(uint8_t);
void    tcpip_getLocalIP(void);
void    tcpip_getStationMac(void);

void    tcpip_getSoftAPIP(void);
void    tcpip_getSoftAPMac(void);
void    tcpip_getStationGateway(void);                                             
void    tcpip_getStationNetmask(void);
void    tcpip_checkInternet(void);


/************************************************************
                        Serverpages
 ************************************************************/

/* Prototypes server pages
 * Leverer og håndterer HTML sider og HTTP GET requests
 */
void tcpip_serverPage(void);
void tcpip_sendHTTPHeader(void);
void tcpip_sendHTTPPage(void);
void toogleGPIO(uint8_t, uint8_t);

/************************************************************/


/**************************************************************
                       TCP/IP Client
 **************************************************************/

const char htmlMsgPart1[] = "<!DOCTYPE html><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;} .button2 {background-color: #77878A;}</style></head><body><h1>ESP8266 Web Server</h1>";

//typedef enum {} 


//AT+CIPSENDBUF
//AT+CIPSEND
//AT+CIPSENDEX
//AT+CIPBUFRESET
//AT+CIPBUFSTATUS
//AT+CIPCHECKSEQ













/**************************************************************/
#endif	/* TCPIP_H */
