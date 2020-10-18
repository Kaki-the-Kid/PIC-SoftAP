/*
 * Header tekst
 *
 */

#ifndef MCC_H
#include "../mcc_generated_files/mcc.h"
#include <string.h>
#endif

#ifndef ESP8266_H
#include "esp8266.h"
#endif

/**************************************************************
                        UART kommandoer 
 **************************************************************/

// AT+UART - UART configuration
/* 
 * Query Command: AT+UART_DEF?
 * Response: 
 * +UART_DEF:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
 * OK
 * 
 * Set Command: 
 * AT+UART_DEF=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
 * Response:
 * OK
 * 
 * Parameter:
 * <baudrate>: UART baud rate
 * <databits>: data bits
 *   5: 5-bit data
 *   6: 6-bit data
 *   7: 7-bit data
 *   8: 8-bit data
 * <stopbits>: stop bits
 *   1: 1-bit stop bit
 *   2: 1.5-bit stop bit
 *   3: 2-bit stop bit
 * <parity>: parity bit
 *   0: None
 *   1: Odd
 *   2: Even
 * <flow control>: flow control
 *   0: flow control is not enabled
 *   1: enable RTS
 *   2: enable CTS
 *   3: enable both RTS and CTS
 * 
 * Notes
 * 1. The configuration changes will be saved in the user parameter area in the
 *    flash, and will still be valid when the chip is powered on again.
 * 2. The use of flow control requires the support of hardware:
 *   MTCK is UART0 CTS
 *   MTDO is UART0 RTS
 * 3. The range of baud rates supported: 110~115200*40.
 * 
 * Example AT+UART_DEF=115200,8,1,0,3
 */
void esp_setUARTConfiguration(void) {
    printf("AT+UART=%d,%d,%d,%d,%d", uart.baudrate, uart.databits, uart.flowcontrol, uart.parity, uart.stopbits);
}

// Se ovenfor ...
void esp_getUARTConfiguration(void) {
    printf("AT+UART?"); // Hent konfiguration
    while(!esp_uartReceive("OK")) {};
}

// Reset ESP01 modulet
void esp_restartModule(void) {
    printf("AT+RST\r\n");
    while(!esp_uartReceive("OK")) {};
}


// AT+GMR?Checks Version Information
/*
 * Response old firmware:
 * AT version:1.2.0.0(Jul  1 2016 20:04:45)
 * SDK version:1.5.4.1(39cb9a32)
 * Ai-Thinker Technology Co. Ltd.
 * Dec  2 2016 14:21:16
 * OK
 * 
 * Response updataed formware:
 * AT version:1.2.0.0(Jul  1 2016 20:04:45)
 * SDK version:1.5.4.1(39cb9a32)
 * Ai-Thinker Technology Co. Ltd.
 * v1.5.4.1-a Nov 30 2017 15:54:51
 * OK
 * 
 * Response new firmware:
 * AT version:1.7.0.0(Aug 16 2018 00:57:04)
 * SDK version:3.0.0(d49923c)
 * compile time:Aug 23 2018 16:58:12
 * Bin version(Wroom 02):v1.7.0
 * OK

 */
void esp_getVersion(void) {
    printf("AT+GMR\r\n");
    while (!esp_uartReceive("OK") || !esp_uartReceive("ERROR")) {};
}


// AT+CIUPDATE - Upgrades the software through network
/*
 * Execute Command: AT+CIUPDATE
 * Function: updates software.
 * 
 * Response:
 * +CIPUPDATE:<n>
 * OK
 * 
 * Parameters:
 * <n>:
 *  - 1: find the server.
 *  - 2: connect to server.
 *  - 3: get the software version.
 *  - 4: start updating.
 * 
 * Notes:
 *  - The speed of the upgrade is susceptible to the connectivity of the network.
 *  - ERROR will be returned if the upgrade fails due to unfavourable network 
 *    conditions. Please wait for some time before retrying.
 * 
 * Notes:
 *  - If using Espressif?s AT BIN (/ESP8266_NONOS_SDK/bin/at), AT+CIUPDATE will 
 *    download a new AT BIN from the Espressif Cloud.
 *  - If using a user-compiled AT BIN, users need to make their own AT+CIUPDATE 
 *    upgrade. Espressif provides a demo as a reference for local upgrade 
 *    (/ESP8266_NONOS_SDK/example/at).
 *  - It is suggested that users call AT+RESTORE to restore the factory default 
 *    settings after upgrading the AT firmware.
 */
void esp_upgradeFirmware(void) {
    printf("AT+CIUPDATE\r\n");
    while (!esp_uartReceive("1") || !esp_uartReceive("ERROR")) {};
    while (!esp_uartReceive("2") || !esp_uartReceive("ERROR")) {};
    while (!esp_uartReceive("3") || !esp_uartReceive("ERROR")) {};
    while (!esp_uartReceive("4") || !esp_uartReceive("ERROR")) {};
    while (!esp_uartReceive("OK") || !esp_uartReceive("ERROR")) {};
}


// AT - Tester om der er forbindelse til AT
void esp_ATTestStartup(void) {
    printf("AT\r\n");
}


// ATE - AT Commands Echoing on/of
/* Execute Command: ATE
 * Response OK
 * 
 * Parameters:
 *  - ATE0: Switches echo off.
 *  - ATE1: Switches echo on.
 * 
 * Note:
 * This command ATE is used to trigger command echo. It means that entered 
 * commands can be echoed back to the sender when ATE command is used. Two 
 * parameters are possible. The command returns OK in normal cases and ERROR 
 * when a parameter other than 0 or 1 was specified.
 */
void esp_echoOff(void) {
    printf("ATE0\r\n");
    while( !esp_uartReceive("OK") || !esp_uartReceive("ERROR") )
        ;
}


/// ATE - AT Commands Echoing on/of
/* Execute Command: ATE
 * Response OK
 * 
 * Parameters:
 *  - ATE0: Switches echo off.
 *  - ATE1: Switches echo on.
 * 
 * Note:
 * This command ATE is used to trigger command echo. It means that entered 
 * commands can be echoed back to the sender when ATE command is used. Two 
 * parameters are possible. The command returns OK in normal cases and ERROR 
 * when a parameter other than 0 or 1 was specified.
 */
void esp_echoOn(void) {
    printf("ATE1\r\n");
    do { esp_waitForOK(); } while( server.busy );
}


/// AT+CWAUTOCONN - Auto-Connects to the AP or Not
/*
 * Set Command AT+CWAUTOCONN=<enable>
 * Response OK
 * Parameters
 * <enable>:
 *  - 0: does NOT auto-connect to AP on power-up.
 *  - 1: connects to AP automatically on power-up.
 * 
 * The ESP8266 Station connects to the AP automatically on power-up by default.
 * 
 * Note:
 * The configuration changes will be saved in the system parameter area in the 
 * flash.
 * 
 * Example:
 * AT+CWAUTOCONN=1
 */
void esp_autoconnect(uint8_t mode) {
    uint8_t AP_AUTOCONNECT = 0;

    printf("AT+CWAUTOCONN=1\r\n");
    //#define AT_CWAUTOCONN_QUERY { printf("AT+CWAUTOCONN=?\r\n"); }
}


void esp_uartInitialize(void) {        
    uart.baudrate    = 155200;
    uart.databits    = 8;
    uart.stopbits    = 1;
    uart.parity      = 0;
    uart.flowcontrol = 1;
}

uint8_t index = 0;
uint8_t esp_uartReceive(char *response){
    char string[256];
    char tmpip[20];
    char tmp;


    if (EUSART1_is_rx_ready()) {
        int i = 0;
        uint8_t index = 0;
        
        for(i=0; i<= 64; i++){
            uint8_t tmp = EUSART1_Read();
            if(tmp != 0x0D && tmp != 0x0A) { // Spring \r og \n over
                if(isprint(tmp)) {
                    string[index++] = tmp;
                }
            }
 
            if(tmp == 0x0A) { // Er vi nået til enden af en linie?
                string[index] = 0; // 0-termineret streng
                
                if( (strcmp(string, "OK") == 0) || (strcmp(string, "no change") == 0) || (strcmp(string, "Send OK") == 0) ) {
                    states.ok = true;
                    states.error = false;
                    states.ready = false;
                    server.busy = false;
                    break; // OK er den sidste kommando sendt fra ESP
                } else if ( strcmp(string, "busy p...") == 0) {
                    server.busy = true;
                    break;
                } else if ( strcmp(string, "ERROR") == 0) {
                    states.ok = false;
                    states.error = true;
                    states.ready = false;
                    break;
                } else if ( strcmp(string, "ready") == 0 ) { 
                    states.ok = false;
                    states.error = false;
                    states.ready = true;
                    break;
                } else if ( strcmp(string, "WIFI DISCONNECT") == 0) {
                    strcpy((char*)server.ssid, "");
                    server.wificonnected = false;
                    server.wifigotip = false;
                    states.ok = false;
                    states.error = true;
                    break;
                } else if ( strcmp(string, "WIFI CONNECTED") == 0 ) {
                    server.wificonnected = true;
                    break;
                } else if ( strcmp(string, "WIFI GOT IP") == 0) {
                    server.wifigotip = true;
                    states.error = false;
                    states.ready = false;
                    break;
                } else if ( strcmp(string, "0,CLOSED") == 0 ) {
                    server.clientConnected = false;
                    break;
                } else if ( strcmp(string, "1,CLOSED") == 0 ) {
                    server.clientConnected = false;
                    break;
                } else if ( strcmp(string, "0,CONNECT") == 0 ) {
                    server.clientConnected = true;
                    break;
                } else if ( strcmp(string, "1,CONNECT") == 0 ) {
                    server.clientConnected = true;
                    break;
                } else if ( strncmp(string, "+IPD" ,4) == 0 ) { //+IPD,0,478:GET / HTTP/1.1
                    server.clientAvailable = true;
                    tcpip_serverPage();
                    break;
                } else if ( strncmp(string, "+CWJAP", 6) == 0 ) { //+CWJAP:"SSID-JRzcM4","00:1e:80:2d:ba:61",10,-62,0
                    led_getrequest_SetHigh();
                } else if ( strncmp(string, "No AP", 5) == 0) { // Vi får No AP efter AT+CWJAB?
                    server.wificonnected = false;
                    server.wifigotip = false;
                    break;
                } else if ( strncmp(string, "+CWLAP", 6) == 0 ) {
                    char src[40];
                    char dest[20];
                    uint8_t length;
                    if( strcmp((char*)server.ssid, "") == 0) {
                        memset(dest, '\0', sizeof(dest));
                        strcpy(src, string+9); // Ser bort fra +CWLAP:("
                        length = (uint8_t) strlen(string)-11; // Klipper ") i enden
                        strncpy(dest, src, length ); 
                        wifi_network_lookup(dest);
                    }
                } else if ( strncmp(string, "+STA_CONNECTED", 14) == 0) {
                    printf("SoftAP Client connected\r\n");
                    break;
                } else if ( strncmp(string, "+CIFSR:STAIP", 12) == 0 ) {
                    //+CIPSTA:ip:"192.168.0.22"
                    //strncpy( server.remote_ip, (string+14), (strlen(string) -14 -2 ) );
                }else if ( strncmp(string, ">", 1) == 0 ) {
                    if ( strncmp(response, ">", 1) == 0 )
                        return true;
                    else 
                        return false;
                    break;
                } else {
                    states.ok=0;
                    states.error=0;
                    states.ready = 0;
                }
                
                index=0;
            }
        }
        
    }

    /*
    // Firmware control
    if ( !strcmp(string, "AT version") ) {
        if ( !strcmp(string, "1.7.0.0") ) {
            firmware.ATVersion = "1.7.0.0";
            firmware.updated = 1;
        } else if ( !strcmp(string, "1.2.0.0") ) {
            firmware.ATVersion = "1.2.0.0";
            firmware.updated = 0;
        } else {
            //something
        }
    } else if ( !strcmp(string, "+UART") ) {
        //Eks. +UART:115273,8,1,0,1
        uart.baudrate = 115200;
        uart.databits = 8;
        uart.flowcontrol = 1;
        uart.parity = 0;
        uart.stopbits = 1;
    } else if ( !strcmp(string, "+CWMODE:") ) { // wifi mode indstilling
        if ( !strcmp(string, ":1") ) {
            wifi.softAPMode  = 0;
            wifi.stationMode = 1;
        } else if( !strcmp(string, ":2") ) {
            wifi.softAPMode  = 1;
            wifi.stationMode = 0;
        } else if( !strcmp(string, ":3") ) {
            wifi.softAPMode  = 1;
            wifi.stationMode = 1;            
        } else {
            // something
        }
    }
    
    if ( strcmp(string, "GET /") == 0 ) {// Har fået en GET til rod request fra klient
        if ( !strcmp(string, "HTTP/1.1") ) { // Er det en gyldig HTTP Resquest?
            //send header + page
            
        } else {
            esp_uartTransmit("Kun HTTP forbindelser");
        }
    }

    if ( strcmp(string, "Upgrade-Insecure-Requests: 1") == 0 ) {// Klienten vil gerne have SSL forbindelse
        //do something
    }

    if ( strcmp(string, "Accept-Language" ) == 0 ) {
        //resolve language
    }

        //sprintf(msg, "GPIO 1 on");
        //LCD_output(msg, 0 ,0);
        //toogleGPIO(1, 1);
    }
    
    if ( strcmp( string, "GET /1/off" ) == 0 ) { // turns the GPIOs on and off
        //sprintf(msg, "GPIO 1 off");
        //LCD_output(msg, 0, 0);
        //toogleGPIO(1,0);
    }    
    
    if ( strcmp(string, "GET /2/on") == 0 ) { // turns the GPIOs on and off
        //sprintf(msg, "GPIO 2 on");
        //LCD_output(msg, 0, 0);
        //toogleGPIO(2, 1);
    } 
    
    if ( strcmp(string, "GET /2/off") == 0 ) { // turns the GPIOs on and off
        //sprintf(msg, "GPIO 5 off");
        //LCD_output(msg, 0, 0);
        //toogleGPIO(2, 0);
    }
    */
    return 1;
}


///AT+
/* Funktion til at sende karakterer til EUSART1
 * printf funktion?!?*/
void esp_uartTransmit(char *msg) {
    //single or multible connections
    printf(msg);
}

bool esp_waitForOK(void) {
    states.ok = false;
    states.error = false;
    
    while( !states.ok && !states.error ) {
        esp_uartReceive("OK");
    }
    
    if(states.ok) {
        return true;
    } else {
        return false;
    }
}


// Funktion som sender til terminal
void esp_terminalReceive(void) {
    //led1_SetHigh();
    //PIR1bits.TX1IF = 0;
}


// Funktion som sender til terminal
void esp_terminalTransmit(char *sendmsg) {
    //led1_SetHigh();
    //PIR1bits.TX1IF = 0;
    printf(sendmsg);
}


void esp_adjustLeds(void) {
    
    if(states.error){
        led_error_Toggle();
    } else {
        led_error_SetLow();
    }
    
    if (states.ok) {
        led_ok_SetHigh();
    } else {
        led_ok_SetLow();
    }
    
    if (states.ready) {
        led_ready_SetHigh();
    } else {
        led_ready_SetLow();
    }
    
    if (server.clientConnected) {
        led_client_connected_SetHigh();
    } else {
        led_client_connected_SetLow();
    }
    
    if ( server.wificonnected == true && server.wifigotip == true )
        led_ap_connected_SetHigh();
    else
        led_ap_connected_SetLow();
    
    // softAP connected
    //led_softap_connected_SetHigh();
    
    // TCP/IP started
    if( server.running) {
        if(!server.internet)
            led_tcp_server_Toggle();
        else
            led_tcp_server_SetHigh();
    }
    else
        led_tcp_server_SetLow();
}

/**************************************************************/

/**
 End of File
*/