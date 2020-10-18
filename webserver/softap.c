/******************************************************************************
 * SoftAP                                                                     *
 ******************************************************************************
 * File:    softap.c                                                          *
 * Author:  krs                                                               *
 * Created: August 31, 2019, 8:03 PM                                          *
 * @brief:                                                                    *
 ******************************************************************************
 *                                                                            *
 * SoftAP is an abbreviated term for "software enabled access point". This is * 
 * software enabling a computer which hasn't been specifically made to be a   *
 * router into a wireless access point. It is often used interchangeably with *
 * the term "virtual router".                                                 *
 *                                                                            *
 * SoftAP is a common method of configuring Wi-Fi products without a display  *
 * or input device, such as a Wi-Fi enabled appliance, home security camera,  *
 * smart home product or any other IoT device. The process typically involves *
 * these steps:                                                               *
 *                                                                            *
 * 1. The headless device turns on a SoftAP Wi-Fi hotspot.                    *
 * 2. The user downloads a product-specific app on a smartphone, and the app  *
 *    then either uses the underlying Android or iOS operating system to      *
 *    connect to the SoftAP hotspot, or directs the user to connect the phone *
 *    manually.                                                               *
 * 3. The app then asks the user for the user's private Wi-Fi network name    *
 *    (SSID) and passkey                                                      *
 * 4. The app sends the SSID and passkey to the headless device over the      *
 *    SoftAP connection.                                                      *
 * 5. The headless device then falls off the SoftAP network and joins the     *
 *    user's private Wi-Fi network.                                           *
 * 
 * This process can work well, but there are two core problems. First, the    *
 * process often requires the user to manually connect to the SoftAP network, * 
 * which can be confusing for mainstream users. Second, if the user enters the* 
 * passkey incorrectly, or if the phone gets disconnected from the SoftAP     *
 * network for any reason, it is difficult for the app and device to smoothly *
 * recover, so the user is often left having to factory reset the device and  *
 * start over. Third, different phones (hardware and OS versions) handle      *
 * SoftAP differently, so the user experience varies dramatically --          *
 * especially with the wide variety of Android hardware and software.         *
 *                                                                            *
 ******************************************************************************/
#pragma once

#ifndef ESP_H
#include "esp8266.h"
#endif

#ifndef SOFTAP_H
#include "softap.h"
#endif


// AT+CWSAP - to configure the ESP8266 SoftAP.
/* 
 * Query Command: AT+CWSAP?
 * Function: to obtain the configuration parameters of the ESP8266 SoftAP.
 * 
 * Set Command: AT+CWSAP_CUR=<ssid>,<pwd>,<chl>,<ecn>[,<max conn>][,<ssid hidden>]
 * Function: to configure the ESP8266 SoftAP.
 * 
 * Response:
 * +CWSAP_CUR:<ssid>,<pwd>,<chl>,<ecn>,<max conn>,<ssid hidden>
 * OK
 *    or
 * ERROR
 * 
 * Parameters:
 * <ssid>: string parameter, SSID of AP.
 * <pwd>: string parameter, length of password: 8 ~ 64 bytes ASCII.
 * <chl>: channel ID.
 * <ecn>: encryption method; WEP is not supported.
 *  - 0: OPEN
 *  - 2: WPA_PSK
 *  - 3: WPA2_PSK
 *  - 4: WPA_WPA2_PSK
 * [<max conn>] (optional): maximum number of Stations to which ESP8266
 *   SoftAP can be connected; within the range of [1, 8].
 * [<ssid hidden>] (optional):
 *  - 0: SSID is broadcasted. (the default setting)
 *  - 1: SSID is not broadcasted.
 * 
 * Notice:
 * This command is only available when SoftAP is active.
 * 
 * Note:
 * The configuration changes will NOT be saved in the flash.
 * 
 * Example:
 * AT+CWSAP="ESP8266","1234567890",5,3
 * 
 * Versions:
 * AT_CWSAP
 * AT_CWSAP_CUR
 * AT_CWSAP_DEF
 */
void softap_SoftAPInit(void) {
    //AT+CWSAP=
    printf("AT+CWSAP=\"%s\",\"%s\",%d,%d,%d,%d\r\n", SOFTAP_SSIDNAME, SOFTAP_PASSWORD, SOFTAP_CHANNELID, SOFTAP_ECN, SOFTAP_MAXCONN, SOFTAP_HIDDEN );
    do { esp_waitForOK(); } while( server.busy );
    
    if( esp_uartReceive("OK") ) {
    } else {
    }
}


// AT+CWLIF - IP of Stations to Which the ESP8266 SoftAP is Connected
/* 
 * Execute Command:
 * AT+CWLIF
 * 
 * Response:
 * <ip addr>,<mac>
 * OK
 * 
 * Parameters:
 * <ip addr>: IP address of Stations to which ESP8266 SoftAP is connected.
 * <mac>: MAC address of Stations to which ESP8266 SoftAP is connected.
 * 
 * Note:
 * This command cannot get a static IP. It only works when both DHCPs of the 
 * ESP8266 SoftAP, and of the Station to which ESP8266 is connected, are enabled.
 */
void softap_getSoftAPConnectedIPs(void) {
    printf("AT+CWLIF\r\n");
}


// AT+CIPAP_CUR?Sets the IP Address of the ESP8266 SoftAP
/*
 * Query Command: AT+CIPAP_CUR?
 * Function: to obtain the current IP address of the ESP8266 SoftAP.
 * 
 * Set Command: AT+CIPAP_CUR=<ip>[,<gateway>,<netmask>]
 * Function: to set the current IP address of the ESP8266 SoftAP.
 * 
 * Response:
 * +CIPAP_CUR:<ip>
 * +CIPAP_CUR:<gateway>
 * +CIPAP_CUR:<netmask>
 * OK
 * OK
 * 
 * Parameters:
 * <ip>: string parameter, the IP address of the ESP8266 SoftAP.
 * [<gateway>]: gateway.
 * [<netmask>]: netmask.
 * 
 * Notes:
 * The configuration changes will not be saved in the flash.
 * Currently, ESP8266 only supports class C IP addresses.
 * The Set Command interacts with DHCP-related AT commands (AT+CWDHCP-related 
 * commands):
 *  - If static IP is enabled, DHCP will be disabled;
 *  - If DHCP is enabled, static IP will be disabled;
 *  - Whether it is DHCP or static IP that is enabled depends on the last configuration.
 * 
 * Example:
 * AT+CIPAP_CUR="192.168.5.1","192.168.5.1","255.255.255.0"
 * 
 * Versions:
 * AT+CIPAP
 * AT+CIPAP_CUR
 * AT+CIPAP_DEF
 */
int softap_getSoftAPIP(void) {
    int ip;
    
    printf( "AT+CIPAP?\r\n" );
    //resolve uart output
    return ip;
}


void softap_setSoftAPIP(void) {
    char SOFTAP_IP[]      = { "192.168.5.1" };
    char SOFTAP_GATEWAY[] = { "192.168.5.1" };
    char SOFTAP_NETMASK[] = { "255.255.255.0"};

    printf( "AT+CIPAP=\"%s\",\"%s\",\"%s\"", SOFTAP_IP, SOFTAP_GATEWAY, SOFTAP_NETMASK );
}

