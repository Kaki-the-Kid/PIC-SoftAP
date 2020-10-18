/**************************************************************
                   I2C generelle kommandoer 
 **************************************************************/
/* 
 * File:   i2c.h
 * Author: krs
 *
 * Created on September 6, 2019, 8:51 AM
 */
#pragma once

#ifndef MCC_H
#include "../mcc_generated_files/mcc.h"
#endif

#ifndef I2C_H
#define	I2C_H

/**************************************************************/

// Global vars
//#define debug

// I2C bus PIC18F26K22 I2C port 2
#define SCL         TRISCbits.RC3
#define SDA         TRISCbits.RC4
#define SCL_IO      ANSELCbits.ANSC3
#define SDA_IO      ANSELCbits.ANSC4

/**************************************************************/

uint8_t data_out[4];

void i2c_init(void);
void i2c_portScan(void);
void i2c_write_serial(uint8_t, uint8_t*, uint8_t);
void i2c_read_serial(uint8_t, uint8_t *, uint8_t);

void i2c_master_wait(void);
void i2c_master_start(void);
void i2c_master_stop(void);
void i2c_master_ack(void);
void i2c_master_nack(void);

/**************************************************************/                
#endif	/* I2C_H */