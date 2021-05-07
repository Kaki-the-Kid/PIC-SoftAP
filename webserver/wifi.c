/**************************************************************
                        WIFI kommandoer 
 **************************************************************/

#ifndef ESP8266_H
#include "esp8266.h"
#endif


// AT+CWMODE - Sets the Current Wi-Fi mode
/* 
 * 1: Station mode
 * 2: SoftAP Mode
 * 3: SoftAP+Station mode
 * 
 * Versions:
 * AT_CWMODE=<mode>
 * AT_CWMODE_CUR=<mode>
 * AT_CWMODE_DEF=<mode>
 */
void wifi_setWifiMode(uint8_t mode) {
    uint8_t CWMODE = 1;
    
    lcd_write("@Station mode", 1 , 1);
    
    
    if(mode > 0 && mode < 4) {
        printf("AT+CWMODE=%d\r\n", mode);
    } else {
        printf("AT+CWMODE=%d\r\n", CWMODE);
    }
    do { esp_waitForOK(); } while( server.busy );
}

// AT+CWMODE? - gets the Current Wi-Fi mode;
void wifi_getWifiMode(void) {
    printf("AT+CWMODE?");
    do { esp_waitForOK(); } while( server.busy );
}


// AT+CIPMODE Configures the transmission mode
/*
 * Query Command: AT+CIPMODE?
 * Function: to obtain information about transmission mode.
 * 
 * Set Command: AT+CIPMODE=<mode>
 * Function: to set the transmission mode.
 * 
 * Response: 
 * +CIPMODE:<mode>
 * OK
 * OK
 * 
 * Parameters:
 * <mode>
 *  - 0: normal transmission mode.
 *  - 1: UART-Wi-Fi passthrough mode (transparent transmission), which can only 
 *       be enabled in TCP single connection mode or in UDP mode when the remote 
 *       IP and port do not change.
 * 
 * Notes:
 *  - The configuration changes will NOT be saved in flash.
 *  - During the UART-Wi-Fi passthrough transmission, if the TCP connection 
 *    breaks, ESP8266 will keep trying to reconnect until +++ is input to exit 
 *    the transmission. If it is a normal TCP transmission and the TCP connection 
 *    breaks, ESP8266 will give a prompt and will not attempt to reconnect.
 * 
 * Example: AT+CIPMODE=1
 */
void wifi_SetTransmissionMode(uint8_t mode) {
    char msg[50];
    
    if(mode == 0 || mode == 1) {
        printf("AT+CIPMODE=%d\r\n", mode);
        if(mode == 0){
            sprintf(msg, "Trans.mode=norm");
        } else if(mode ==1) {
            sprintf(msg, "Trans.mode=P.thr");
        }
        //LCD_output(msg, 0, 0);
        
        //return 1; // Success
    } else {
        //return 0; // Fail
    }
}


// AT+CWLAB - List All APs
/*
 * Indstillinger laves med AT_CWLAPOPT
 * 
 * Response:
 * +CWLAP:(3,"SSID-gpP9xc",-67,"00:1e:80:23:cf:69",1,3,0)
 * +CWLAP:(3,"SSID-4rgfwF",-65,"00:1e:80:2d:b9:89",1,10,0)
 * +CWLAP:(3,"SSID-9cbL47",-69,"00:1e:80:2a:2c:cd",1,11,0)
 * +CWLAP:(0,"Libratone 25FDF7",-71,"00:07:f5:25:fd:f7",6,6,0)
 * +CWLAP:(3,"SSID-JRzcM4",-43,"00:1e:80:2d:ba:61",11,13,0)
 * 
 * 
 * +CWLAP:<ecn>,<ssid>,<rssi>,<mac>,<channel>,<freq offset>, <freq cali>,
 * <pairwise_cipher>,<group_cipher>,<bgn>,<wps>
 * 
 * OK
 * 
 * 
 * 
 * AT+CWLAPOPT - Sets the Configuration for the Command AT+CWLAP
 *
 * AT+CWLAPOPT=<sort_enable>,<mask>
 * Response:
 * OK
 *    or
 * ERROR
 * 
 * Parameters
 * ? <sort_enable>: determines whether the result of command AT+CWLAP will be 
 * listed according to
 * 
 * RSSI:
 * 0: the result is not ordered according to RSSI.
 * 1: the result is ordered according to RSSI.
 * 
 * ? <mask>: determines the parameters shown in the result of AT+CWLAP; 
 * 0 means not showing the parameter corresponding to the bit, and 1 means 
 * showing it.
 * bit 0: determines whether <ecn> will be shown in the result of AT+CWLAP.
 * bit 1: determines whether <ssid> will be shown in the result of AT+CWLAP.
 * bit 2: determines whether <rssi> will be shown in the result of AT+CWLAP.
 * bit 3: determines whether <mac> will be shown in the result of AT+CWLAP.
 * bit 4: determines whether <ch> will be shown in the result of AT+CWLAP.
 * bit 5: determines whether <freq offset> will be shown in the result of AT+CWLAP.
 * bit 6: determines whether <freq calibration> will be shown in the result of AT+CWLAP.
 * bit 7: determines whether <pairwise_cipher> will be shown in the result of AT+CWLAP.
 * bit 8: determines whether <group_cipher> will be shown in the result of AT+CWLAP.
 * bit 9: determines whether <bgn> will be shown in the result of AT+CWLAP.
 * bit 10: determines whether <wps> will be shown in the result of AT+CWLAP.
 * 
 * Example
 * AT+CWLAPOPT=1,2047
 * The first parameter is 1, meaning that the result of the command AT+CWLAP 
 * will be ordered according to RSSI;
 * 
 * The second parameter is 2047, namely 0x7FF, meaning that the corresponding 
 * bits of <mask> are set to 1. All parameters will be shown in the result of AT+CWLAP.
 * 
 */
void wifi_network_request(void) {
    printf("AT+CWLAPOPT=1,2\r\n");
    esp_waitForOK();
    
    printf("AT+CWLAP\r\n");
    do { esp_waitForOK(); } while( server.busy );
}

//
/*
 */
bool wifi_network_lookup(const char *ssid_name) {
    knownnetworks* ptr    = network_table;
    knownnetworks* endPtr = network_table + sizeof(network_table)/sizeof(network_table[0]);
    
    while (ptr < endPtr)
    {
        if (strcmp(ptr->ssid, ssid_name) == 0) 
        {
            strcpy((char*)server.ssid, ptr->ssid);
            strcpy((char*)server.password, ptr->password);
            strcpy((char*)server.encryption, ptr->encryption);
            
            return true;
        }
        
        ptr++;
    }
    
    return false;
}


// AT_CWJAP - Forbinder til AP
/*  
 * Viser sig på LAN som: 
 * Hostname:   ESP_0B81FB, 
 * MAC:        2c:3a:e8:0b:81:fb
 * 
 * Response:
 * WIFI CONNECTED
 * WIFI GOT IP
 * 
 * OK
 *
 */
void wifi_connectToAP(void) {
    char msg[16];
    lcd_write("@Forbinder ...  ", 1, 0);
    
    wifi_network_request();
    do { esp_waitForOK(); } while( server.busy );
    
    printf("AT+CWJAP=\"%s\",\"%s\"\r\n", server.ssid, server.password); //AT+CWJAP="SSID-JRzcM4","frbPCwvRKq";
    while ( !server.wificonnected && !server.wifigotip ) { esp_uartReceive(0); }
    do { esp_waitForOK(); } while( server.busy );
   
    
    // Print local IP address and start web server
    lcd_write((char*)server.ssid, 1, 0);
    sprintf(msg, "@IP: %s", server.remote_ip);
    lcd_write(msg, 2, 1);
}


// AT+CWQAP - Disconnects from the AP
/* 
 * Execute Command: AT+CWQAP
 * Response:
 * 
 * OK
 * 
 * Parameters:
 *  -
 */
void wifi_disconnectFromAP(void) {
    printf("AT+CWQAP\r\n");
    while(!esp_uartReceive("OK"));
    while(!esp_uartReceive("WIFI DISCONNECT"));                                                     
}

//
//AT+CWJAP?                                
//+CWJAP:"ASUS_X00PD","40:b0:76:7a:ac:de",11,-57,0
//                                                                                
//OK
// string+7
//  const char s[2] = "-";
void wifi_isConnectedAP(){
    printf("AT+CWJAP?\r\n");
    esp_waitForOK();
    //No AP    
}


// AT+CIPSTATUS Gets the connection status
/*
 * Execute Command: AT+CIPSTATUS
 * Response STATUS:<stat>
 * 
 * +CIPSTATUS:<link ID>,<type>,<remote IP>,<remote port>,<local port>,<tetype>
 * 
 * Parameters:
 * <stat>: status of the ESP8266 Station interface.
 *  - 2: The ESP8266 Station is connected to an AP and its IP is obtained.
 *  - 3: The ESP8266 Station has created a TCP or UDP transmission.
 *  - 4: The TCP or UDP transmission of ESP8266 Station is disconnected.
 *  - 5: The ESP8266 Station does NOT connect to an AP.
 * <link ID>: ID of the connection (0~4), used for multiple connections.
 * <type>: string parameter, "TCP" or "UDP".
 * <remote IP>: string parameter indicating the remote IP address.
 * <remote port>: the remote port number.
 * <local port>: ESP8266 local port number.
 * <tetype>:
 *  - 0: ESP8266 runs as a client.
 *  - 1: ESP8266 runs as a server.
 */
void wifi_getConnectionStatus(void) {
}


/************************************************************/

/**
 End of File
*/
