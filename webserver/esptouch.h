/* 
 * File:   esptouch.h
 * Author: krs
 *
 * Created on August 31, 2019, 10:02 PM
 *
 * Inspiration:
 * Smartconfig of ESP8266, using AT Commands and ESP-Touch:
 * https://www.youtube.com/watch?v=IGOyHBOiWAY
 *
 * Espressif - ESP-Touch
 * https://www.espressif.com/en/products/software/esp-touch/overview
 */

#ifndef ESPTOUCH_H
#define	ESPTOUCH_H

/**************************************************************/                


/*
 * Espressif?s ESP-TOUCH protocol implements the Smart Config technology to 
 * help users connect ESP8266EX-embedded devices to a Wi-Fi network through 
 * simple configuration on a smartphone.
 * 
 * Android App:
 * ESP8266
 * 
 */

/*
 * AT+CWSTARTSMART -Starts SmartConfig
 * 
 * Execute Command:
 * AT+CWSTARTSMART
 * Function: to start SmartConfig. (The type of SmartConfig is ESP-TOUCH + AirKiss.?
 * 
 * Set Command:
 * AT+CWSTARTSMART=<type>
 * Function: to start SmartConfig of a designated type.
 * Response: OK
 * 
 * Parameters:
 * <type>:
 *  - 1: ESP-TOUCH
 *  - 2: AirKiss
 *  - 3: ESP-TOUCH+AirKiss
 * 
 * Messages:
 * When smartconfig starts, it will prompt messages as below:
 * smartconfig type: <type> // AIRKISS or ESPTOUCH
 * 
 * smart get wifi info // got SSID and password
 * ssid:<AP's SSID>
 * password:<AP's password>
 * 
 * // ESP8266 will try to connect to the AP
 * WIFI CONNECTED
 * WIFI GOT IP
 * smartconfig connected wifi 
 * // if the connection failed, it will prompt "smartconfig connect fail"
 * 
 * Notes:
 * ? For details on SmartConfig please see ESP-TOUCH User Guide.
 * ? SmartConfig is only available in the ESP8266 Station mode.
 * ? The message smart get wifi info means that SmartConfig has successfully acquired the AP information. ESP8266 will try to connect to the target AP.
 * ? Message smartconfig connected wifi is printed if the connection is successful. Use command AT+CWSTOPSMART to stop SmartConfig before running other commands. Please make sure that you do not execute other commands during SmartConfig.
 * ? Starting from AT_v1.0, SmartConfig can get protocol type (AirKiss or ESP-TOUCH) automatically by command AT+CWSTARTSMART.
 * ? Users can remove this function to reduce bin size and save memory by recompiling the at project, refer to Section 1.1, and disable the #define CONFIG_AT_SMARTCONFIG_COMMAND_ENABLE in user_config.h.
 * 
 * Example:
 * AT+CWMODE=1
 * AT+CWSTARTSMART
 */
#define AT_CWSTARTSMART { printf("AT+CWSTARTSMART\r\n"); }


/*
 * AT+CWSTOPSMART?Stops SmartConfig
 * Execute Command:
 * AT+CWSTOPSMART
 * Response: OK
 * Parameters -
 * 
 * Note:
 * Irrespective of whether SmartConfig succeeds or not, before executing any other AT commands, please always call AT+CWSTOPSMART to release the internal memory taken up by SmartConfig.
 *
 * Example:
 * AT+CWSTOPSMART
 */
#define  AT_CWSTOPSMART { printf("AT+CWSTOPSMART\r\"); }


/**************************************************************/                
#endif	/* ESPTOUCH_H */