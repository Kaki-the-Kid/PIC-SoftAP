/*
 * Device            :  PIC18F26K22
 * Driver Version    :  2.00
*/

#ifndef I2C_H
    #include "i2c.h"
#endif


//#define debug

/**************************************************************
                   I2C generelle kommandoer 
 **************************************************************/
#ifndef I2C_C

void i2c_init(void) {
#ifdef debug
    printf("Initialiserer I2C bussen\r\n");
#endif
	SCL = SDA = 1;
	SCL_IO = 0;
	SDA_IO = 0;
    
	// bit 5  Synchronous Serial Port Enable bit
	// bit3-0 bestemmer måden hvorpå clocken defineres
	// 1000 = I2C Master mode, clock = FOSC / (4 * (SSPxADD+1))
	
    SSP1CON1 = 0x28;//0b00101000; // 0x28
	
    uint8_t i2c_clock = (_XTAL_FREQ /(4 * 100000)) - 1; // Sætter hastigheden til 100K for I2C bussen
	SSP1ADD  = i2c_clock; // 0x9F ved 16MHz x 4 software pll
	
    SSP1CON2 = 0; //Resetter kontrol register SSPxCON2: SSPx CONTROL REGISTER 2
}

void i2c_portScan(void) {
    for(uint8_t i=0 ; i<0x7F; i++) {
        printf("Scanner addresse: 0x%02X", i);
        i2c_write_serial(i, 0, 1);
    }       
}


void i2c_write_serial(uint8_t addr, char* data, uint8_t length) {
    char msg[50];
    
    uint8_t targetAddr = (addr<<1); // Laver addressen om til 8 bit med WRITE condition
    #ifdef debug
        printf("\r\n***********WRITE****************\r\n");
    #endif
    
    i2c_master_wait();
    i2c_master_start();
    
    SSP1BUF = targetAddr;
    while(SSP1STATbits.R_nW);

    if( SSP1CON2bits.ACKSTAT == 0 ) {
        #ifdef debug
            printf("ACK - Skrev adresse: 0x%02X\r\n", targetAddr);
        #endif

        for (uint8_t i=0; i < length ; i++) {
            #ifdef debug
                printf("Data: 0x%02X\r\n", data[i]);
            #endif
            SSP1BUF = (uint8_t) data[i];
            while(SSP1STATbits.R_nW);

            if( SSP1CON2bits.ACKSTAT == 0 ) { // Hvis vi får en Acknowledge
                #ifdef debug
                    printf("ACK - Skrev data: 0x%02X - %c\r\n", data[i], data[i]);
                #endif
            } else {
                #ifdef debug
                    printf("NACK - gensender\r\n");
                #endif
                SSP1CON2bits.RCEN=1;
                while(SSP1CON2bits.RCEN==1);

                SSP1BUF = (uint8_t) data[i];
                while(SSP1STATbits.R_nW);

                if(SSP1CON2bits.ACKSTAT == 0) {
                    #ifdef debug
                        printf("ACK - Skrev data: 0x%02X - %c\r\n", data[i], data[i]);
                    #endif
                } else {
                    #ifdef debug
                        states.error = true;
                        printf("NACK - Fejl i skrivning af data\r\n");
                    #endif
                }
            }
        }
        
        i2c_master_stop();

    } else {
        #ifdef debug
            printf("Kunne ikke finde enhed p&#230; adresse: 0x%02X %u\r\n", targetAddr, targetAddr);
        #endif
    }
    #ifdef debug
        printf("********************************\r\n");
    #endif
}


void i2c_read_serial(uint8_t addr, uint8_t *data_out, uint8_t length) {
    char msg[50];
    uint8_t targetAddr = ((addr<<1)+1); // Laver addressen om til 8 bit med READ condition
    
    i2c_master_wait();
    i2c_master_start();
    
    SSP1BUF = targetAddr; // Skriver sdressen
	while(SSP1STATbits.R_nW) {};
    
    if(SSP1CON2bits.ACKSTAT == 0) {
        for (uint8_t i=0; i < length ; i++) {
            SSP1CON2bits.RCEN = 1;
            while(SSP1CON2bits.RCEN) {};
            
            data_out[i] = SSP1BUF;
            
            // Sender ACK eller NACK til sidst
            if( i != length -1)// Sender ACK eller NACK til sidst
                i2c_master_ack();
            else
                i2c_master_nack();
           
            SSP1CON2bits.ACKEN = 1;
            while(SSP1CON2bits.ACKEN == 1){};
            
            #ifdef debug
                printf("Laeste: 0x%02X %c\n\r", data_out[i], data_out[i]);
            #endif
        }
        
        // Sender NACK
        i2c_master_nack();
        i2c_master_stop();

    } else {
        #ifdef debug
            esp_terminalTransmit("Kunne ikke finde enhed\r\n");
        #endif
    }
    #ifdef debug
        esp_terminalTransmit("********************************\r\n");
    #endif
}


/* Fra datablad:
 * bit 2 R/W: Read/Write bit information (I2C mode only)
 * This bit holds the R/W bit information following the last address match. 
 * This bit is only valid from the address match
 * to the next Start bit, Stop bit, or not ACK bit.
 *
 * In I2 C Master mode:
 * 1 = Transmit is in progress
 * 0 = Transmit is not in progress
 * OR-ing this bit with SEN, RSEN, PEN, RCEN or ACKEN will indicate if the MSSPx is in Idle mode.
 *
 */
void i2c_master_wait(void) {
    while ( (SSP1STAT & 0x04) || (SSP1CON2 & 0x1F) ) {};
}


void i2c_master_start(void) {
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN == 1){};
}


void i2c_master_stop(void) {
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN == 1){};
}


void i2c_master_ack(void) {
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN == 1){};
}


void i2c_master_nack(void) {
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN == 1){};
}

/**************************************************************/
#endif /*I2C_C*/












