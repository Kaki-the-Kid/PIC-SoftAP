/* 
 * File:   wps.h
 * Author: krs
 *
 * Created on August 31, 2019, 10:24 PM
 */
#pragma once

#ifndef WPS_H
#define	WPS_H

/**************************************************************/

/* 
 * AT+WPS - Enables the WPS Function
 * 
 * Har du en trådløs enhed, der har WPS-funktion, f.eks. en netværksprinter, 
 * og du ønsker at forbinde den til dit trådløse netværk via WPS, skal du 
 * først aktivere WPS-knappen i dit modem (router) og herefter på enheden. 
 * Så oprettes forbindelse, uden at du skal vælge netværket eller indtaste 
 * adgangskode.
 * 
 * 
 * Set Command:
 * AT+WPS=<enable>
 * 
 * Response: OK
 * 
 * Parameters:
 * <enable>:
 *  - 1: enables WPS/Wi-Fi Protected Setup
 *  - 0: disables WPS
 * 
 * Notes:
 * ? WPS must be used when the ESP8266 Station is enabled.
 * ? WPS does not support WEP/Wired-Equivalent Privacy encryption.
 * 
 * Example:
 * AT+CWMODE=1
 * AT+WPS=1
 */
#define AT_WPS { printf("AT+CWMODE=1\n"); printf("AT+WPS=1\n"); }

/**************************************************************/                
#endif	/* WPS_H */