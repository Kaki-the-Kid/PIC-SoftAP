/**************************************************************
                            SSL
 **************************************************************/
/* 
 * File:   ssh.h
 * Author: krs
 *
 * Created on March 8, 2020, 11:08 PM
 */
#pragma once

#ifndef SSL_H
#define	SSL_H

/**************************************************************/                

//AT+CIPSSLSIZE Sets the size of SSL buffer
/*
 * Set Command: AT+CIPSSLSIZE=<size>
 * Response: OK
 * 
 * Parameters: <size> the size of the SSL buffer; range of value: [2048, 4096].
 * 
 * Example: AT+CIPSSLSIZE=4096
 */


// AT+CIPSSLCONF Set configuration of ESP SSL client
/*
 * Commands
Query Command:
AT+CIPSSLCCONF?
Function: Get configuration of the ESP8266 SSL
client.
Set Command:
AT+CIPSSLCCONF=<SSL mode>
Function: Set configuration of the ESP8266 SSL
client.
Response
+CIPSSLCCONF:<SSL mode>
OK
OK
Parameters
<SSL mode>:
? bit0?if set to be 1, certificate and private key will be enabled, so SSL server can verify ESP8266; if
0, then will not.
? bit1?if set to be 1, CA will be enabled, so ESP8266 can verify SSL server; if 0, then will not.
Notes
? If certificates need to be enabled, please call this command before SSL connection is established.
? If certificates need to be enabled, please refer to the ESP8266 SSL User Guide to generate
certificates.
- esp_ca_cert.bin downloads to 0xFB000 by default
- esp_cert_private_key.bin downloads to 0xFC000 by default
- Users can revise the SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR and
SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR in user_main.c to change the
downloading addresses.
? This configuration will be saved in the flash user parameter area.
Example
AT+CWMODE=1 // enable sta mode
AT+CWJAP="SSID","PASSWORD" // connect to an AP
AT+CIPSNTPCFG=1,8 // set SNTP timezone
AT+CIPSNTPTIME? // get SNTP time
AT+CIPSSLCCONF=2
AT+CIPSTART="SSL","192.168.3.38",8443
 */


/**************************************************************/
#endif	/*SSH_H */