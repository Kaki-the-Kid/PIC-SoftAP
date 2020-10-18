/* 
 * File:   softap.h
 * Author: krs
 *
 * Created on 4. marts 2020, 19:28
 */

/**************************************************************
                        SoftAP kommandoer 
 **************************************************************/
/* ESP01 bliver til et Access Point, som kan tilgåes trådløst af 
 * eks. mobil via wifi */

#ifndef MCC_H
#include "mcc_generated_files/mcc.h"
#endif

#ifndef SOFTAP_H
#define	SOFTAP_H

/**************************************************************/                

// SoftAP indstllinger
const char   *SOFTAP_SSIDNAME  = "KRS_PIC_AP";
const char   *SOFTAP_PASSWORD  = "1234567890"; //Minimun 8 tegn
const uint8_t SOFTAP_CHANNELID = 3;
const uint8_t SOFTAP_ECN       = 4;
const uint8_t SOFTAP_MAXCONN   = 1;
const uint8_t SOFTAP_HIDDEN    = 0;

/**************************************************************/                

void softap_SoftAPInit(void);
void softap_getSoftAPConnectedIPs(void);
int  softap_getSoftAPIP(void);
void softap_setSoftAPIP(void);

/**************************************************************/                
#endif	/* SOFTAP_H */