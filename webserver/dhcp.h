/* 
 * File:   dhcp.h
 * Author: krs
 *
 * Created on March 8, 2020, 11:07 PM
 */
#pragma once

#ifndef DHCP_H
#define	DHCP_H

/**************************************************************
                           DHCP
 **************************************************************/

void setupDHCP(void);
void esp_setSoftAPDHCPIP(uint8_t *, uint8_t *, uint8_t, uint8_t );

/**************************************************************/                
#endif	/* DHCP_H */
/*
 End of File
*/
