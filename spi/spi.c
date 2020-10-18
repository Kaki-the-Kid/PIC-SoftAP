/*
 * Header tekst
 *
 */

#ifndef SPI_H
#include "spi.h"
#endif

void spi_init(void)
{
    // todo
}

void spi_master_test(void) {
    //SpiAttr hSpiAttr;
    //hSpiAttr.bitOrder = SpiBitOrder_MSBFirst;
    //hSpiAttr.speed = SpiSpeed_10MHz;
    //hSpiAttr.mode = SpiMode_Master;
    //hSpiAttr.subMode = SpiSubMode_0;
    
    // Init HSPI GPIO
    //WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105);
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2);//configure io to spi mode
    
    //SPIInit(SpiNum_HSPI, &hSpiAttr);
    
    uint32_t value = 0xD3D4D5D6;
    uint32_t sendData[8] ={ 0 };
    
    //SpiData spiData;
    
    printf("\r\n ============= spi init master =============\r\n");
    
    // Test 8266 slave.Communication format: 1byte command + 1bytes address + x bytes Data.
    printf("\r\n Master send 32 bytes data to slave(8266)\r\n");
    //os_memset(sendData, 0, sizeof(sendData));
    //sendData[0] = 0x55565758;
    //sendData[1] = 0x595a5b5c;
    //sendData[2] = 0x5d5e5f60;
    //sendData[3] = 0x61626364;
    //sendData[4] = 0x65666768;
    //sendData[5] = 0x696a6b6c;
    //sendData[6] = 0x6d6e6f70;
    //sendData[7] = 0x71727374;
    
    //spiData.cmd = MASTER_WRITE_DATA_TO_SLAVE_CMD;
    //spiData.cmdLen = 1;
    //spiData.addr = &value;
    //spiData.addrLen = 4;
    //spiData.data = sendData;
    //spiData.dataLen = 32;
    //SPIMasterSendData(SpiNum_HSPI, &spiData);
    
    //printf("\r\n Master receive 24 bytes data from slave(8266)\r\n");
    //spiData.cmd = MASTER_READ_DATA_FROM_SLAVE_CMD;
    //spiData.cmdLen = 1;
    //spiData.addr = &value;
    //spiData.addrLen = 4;
    //spiData.data = sendData;
    //spiData.dataLen = 24;
    //os_memset(sendData, 0, sizeof(sendData));
    //SPIMasterRecvData(SpiNum_HSPI, &spiData);
    
    //printf(" Recv Slave data0[0x%08x]\r\n", sendData[0]);
    //printf(" Recv Slave data1[0x%08x]\r\n", sendData[1]);
    //printf(" Recv Slave data2[0x%08x]\r\n", sendData[2]);
    //printf(" Recv Slave data3[0x%08x]\r\n", sendData[3]);
    //printf(" Recv Slave data4[0x%08x]\r\n", sendData[4]);
    //printf(" Recv Slave data5[0x%08x]\r\n", sendData[5]);
    
    // read the value of slave status register
    //value = SPIMasterRecvStatus(SpiNum_HSPI);
    //printf("\r\n Master read slave(8266) status[0x%02x]\r\n", value);
    
    // write 0x99 into the slave status register
    //SPIMasterSendStatus(SpiNum_HSPI, 0x99);
    //printf("\r\n Master write status[0x99] to slavue(8266).\r\n");
    //SHOWSPIREG(SpiNum_HSPI);
    
    // Test others slave.Communication format:0bytes command + 0 bytes address + x bytes Data
    /*#if 0
        printf("\r\n Master send 4 bytes data to slave\r\n");
        os_memset(sendData, 0, sizeof(sendData));
        sendData[0] = 0x2D3E4F50;
        spiData.cmd = MASTER_WRITE_DATA_TO_SLAVE_CMD;
        spiData.cmdLen = 0;
        spiData.addr = &addr;
        spiData.addrLen = 0;
        spiData.data = sendData;
        spiData.dataLen = 4;
        SPIMasterSendData(SpiNum_HSPI, &spiData);
        printf("\r\n Master receive 4 bytes data from slaver\n");
        spiData.cmd = MASTER_READ_DATA_FROM_SLAVE_CMD;
        spiData.cmdLen = 0;
        spiData.addr = &addr;
        spiData.addrLen = 0;
        spiData.data = sendData;
        spiData.dataLen = 4;
        os_memset(sendData, 0, sizeof(sendData));
        SPIMasterRecvData(SpiNum_HSPI, &spiData);
        printf(" Recv Slave data[0x%08x]\r\n", sendData[0]);
    #endif*/
}

void spi_slave_test() {
    // SPI initialization configuration, speed = 0 in slave mode
    //SpiAttr hSpiAttr;
    //hSpiAttr.bitOrder = SpiBitOrder_MSBFirst;
    //hSpiAttr.speed = 0;
    //hSpiAttr.mode = SpiMode_Slave;
    //hSpiAttr.subMode = SpiSubMode_0;
    
    // Init HSPI GPIO
    //WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105);
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2);//configure io to spi mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2);//configure io to spi mode
    
    //printf("\r\n ============= spi init slave =============\r\n");
    //SPIInit(SpiNum_HSPI, &hSpiAttr);
    
    // Set spi interrupt information.
    //SpiIntInfo spiInt;
    //spiInt.src = (SpiIntSrc_TransDone | SpiIntSrc_WrStaDone |SpiIntSrc_RdStaDone |SpiIntSrc_WrBufDone |SpiIntSrc_RdBufDone);
    //spiInt.isrFunc = spi_slave_isr_sta;
    //SPIIntCfg(SpiNum_HSPI, &spiInt);
    // SHOWSPIREG(SpiNum_HSPI);
    //SPISlaveRecvData(SpiNum_HSPI);
    //uint32_t sndData[8] = { 0 };
    //sndData[0] = 0x35343332;
    //sndData[1] = 0x39383736;
    //sndData[2] = 0x3d3c3b3a;
    //sndData[3] = 0x11103f3e;
    //sndData[4] = 0x15141312;
    //sndData[5] = 0x19181716;
    //sndData[6] = 0x1d1c1b1a;
    //sndData[7] = 0x21201f1e;
    
    // write 8 word (32 byte) data to SPI buffer W8~W15
    //SPISlaveSendData(SpiNum_HSPI, sndData, 8);
    
    // set the value of status register
    //WRITE_PERI_REG(SPI_RD_STATUS(SpiNum_HSPI), 0x8A);
    //WRITE_PERI_REG(SPI_WR_STATUS(SpiNum_HSPI), 0x83);
}


/**
 End of File
*/
