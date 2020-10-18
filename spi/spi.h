/* 
 * File:   spi.h
 * Author: krs
 *
 * Created on September 1, 2019, 5:04 PM
 *
 * Inspiration
 * Wikipedia: Serial Peripheral Interface-bus
 * https://da.wikipedia.org/wiki/Serial_Peripheral_Interface-bus
 *
 * Corelis: SPI Tutorial
 * https://www.corelis.com/education/tutorials/spi-tutorial/
 *
 * SparkFun: Serial Peripheral Interface (SPI)
 * https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all
 */

#pragma once

#ifndef MCC_H
#include "mcc_generated_files/mcc.h"
#endif

#ifndef SPI_H
#define	SPI_H

void spi_init(void);
void spi_master_test(void);
void spi_slave_test(void);


#endif	/* SPI_H */

