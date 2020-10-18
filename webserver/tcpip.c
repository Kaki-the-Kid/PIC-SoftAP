/*
 * Multiple Connections as TCP Server
 * 
 * When ESP8266 is working as a TCP server, the multiple connections should be 
 * allowed, since there may be more than one client connecting to the ESP8266.
 * Here is an example showing how TCP server is realized when ESP8266 is working 
 * in softAP mode.
 */

#ifndef MCC_H
    #include "../mcc_generated_files/mcc.h"
#endif

#ifndef TCPIP_H
    #include "tcpip.h"
#endif

#ifndef ESP8266_H
#include "esp8266.h"
#endif

#ifndef I2C_H
#include "i2c.h"
#endif

#ifndef LCD_H
#include "lcd.h"
#endif

#include <string.h>

/**************************************************************
                TCP/IP Server - single connection 
 **************************************************************/
#ifndef SERVER_C


void tcpip_serverSetup(void) {
    char buffer[50] = "";
    
    sprintf(buffer, "@Connecting to:  ");
    lcd_write(buffer, 1, 0);
    sprintf(buffer, "@%s", server.ssid);
    lcd_write(buffer, 2, 0);
    
    // Vi kan kun oprette en server ved multiple connections activated
    // Multible connections er et krav ved opstart af server
    //tcpip_setConnectionType(1);
    
    // Sæt maksimum antal forbindelser til TCP/IP server
    tcpip_setMaxConnections(1);
    
    // Vis ikke host ip ved +IPD
    tcpip_showIPDRemoteIP(0); 
    
    //setDNSDomain();
 
    // Start en TCP server port 80
    tcpip_serverTCPIPBegin(80); // default port = 333
    if( esp_waitForOK() )
        server.running = true;
    else
        server.running = false;
}


// AT+CIPSERVER - Deletes/Creates a TCP server
/*
 * Set Command AT+CIPSERVER=<mode>[,<port>]
 * 
 * Response: OK
 * 
 * Parameters:
 * ? <mode>:
 *  - 0: deletes server.
 *  - 1: creates server.
 * ? <port>: port number; 333 by default.
 * 
 * Notes
 *  - A TCP server can only be created when multiple connections are activated 
 *    (AT+CIPMUX=1).
 *  - A server monitor will automatically be created when the TCP server is 
 *    created.
 *  - When a client is connected to the server, it will take up one connection 
 *    and be assigned an ID.
 * 
 * Example: 
 * AT+CIPMUX=1
 * AT+CIPSERVER=1,1001
 */
bool tcpip_serverTCPIPBegin(uint16_t port) {
    if (port==80 || port==333) {
        printf("AT+CIPSERVER=1,%d\r\n", port);
        if( esp_waitForOK() ) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


void tcpip_serverTCPIPStop(void) {
    printf("AT+CIPSERVER=0\r\n");
    esp_waitForOK();
}


// AT+CIPMUX Configures the single or multiple connections mode
/*
 * Query Command: AT+CIPMUX?
 * 
 * Set Command: AT+CIPMUX=<mode>
 * 
 * Function: to set the connection type.
 * 
 * Response:
 * +CIPMUX:<mode>
 * OK
 * OK
 * 
 * Parameters:
 * <mode>:
 *  - 0: single connection
 *  - 1: multiple connections
 * 
 * Notes:
 * ? The default mode is single connection mode.
 * ? Multiple connections can only be set when transparent transmission is 
 *   disabled (AT+CIPMODE=0).
 * ? This mode can only be changed after all connections are disconnected.
 * ? If the TCP server is running, it must be deleted (AT+CIPSERVER=0) before 
 *   the single connection mode is activated.
 * 
 * Example: AT+CIPMUX=1
 */
void tcpip_setConnectionType(uint8_t multibleConnections) {
    server.multibleConnections = multibleConnections;

    printf("AT+CIPMUX=%d\r\n", multibleConnections);
    do { esp_waitForOK(); } while( server.busy );
}


// AT+CIPSERVERMAXCONN - Set the maximum connections that server allows
/*
 * Query Command: * AT+CIPSERVERMAXCONN?
 * Function: obtain the maximum number of clients allowed to connect to the TCP 
 * or SSL server.
 * 
 * Set Command: AT+CIPSERVERMAXCONN=<num>
 * Function: set the maximum number of clients allowed to connect to the TCP or 
 * SSL server.
 * 
 * Response +CIPSERVERMAXCONN:<num>
 * OK
 * OK
 * Parameters: <num>: the maximum number of clients allowed to connect to the 
 * TCP or SSL server, range: [1, 5]
 * 
 * Notes:
 * To set this configuration, you should call the command 
 * AT+CIPSERVERMAXCONN=<num> before creating a server.
 * 
 * Example:
 * AT+CIPMUX=1
 * AT+CIPSERVERMAXCONN=2
 * AT+CIPSERVER=1,80
 */
bool tcpip_setMaxConnections(uint8_t conn) {
    if (conn>=1 && conn <=5 ) {
        printf("AT+CIPSERVERMAXCONN=%d\r\n", conn);
        if( esp_waitForOK() )
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}


// AT+CIPRECVLEN - Get TCP Data Length in Passive Receive Mode
/*
 * Query Commands AT+CIPRECVLEN?
 * 
 * Response:
 * +CIPRECVLEN:<data length of link0>,<data length of link1>,
 * <data length of link2>,<data length of link3>,<data length of link4>
 * OK
 * 
 * Parameters:
 * <data length of link>: length of the entire data buffered for the link
 * 
 * Example:
 * AT+CIPRECVLEN?
 * +CIPRECVLEN:100,,,,,
 * OK
 */
uint8_t dataLengthCon1 = 0;
uint8_t dataLengthCon2 = 0;
uint8_t dataLengthCon3 = 0;
uint8_t dataLengthCon4 = 0;
uint8_t dataLengthCon5 = 0;

void tcpip_getDataLength(void) {
    //int input;
    
    printf("AT+CIPRECVLEN?\r\n");
    //input = esp_uartReceive();
}


// AT+CIPCLOSEMODE - Set the Close Mode of TCP Connection
/*
 * Commands
 * 1. Single connection: (+CIPMUX=0) AT+CIPCLOSEMODE=<enable_abort>
 * 2. multiple connections: (+CIPMUX=1) AT+CIPCLOSEMODE=<link ID>,<enable_abort>
 * 
 * Response: OK
 * 
 * Parameters:
 * <link ID>: ID of the connection
 * <enable_abort>: 0, normal disconnect; 1, abort to disconnect
 * 
 * Notes:
 * ? Default is the normal disconnect mode. Usually the abort mode should not be 
 *   enabled.
 * ? If the abort mode is needed, please use this command after TCP connection 
 *   is established. And the configuration only takes effect in the current 
 *   connection. If the connection breaks, you need to set it again after new 
 *   connection is established.
 * ? This configuration only works on normal TCP connection, cannot be used on SSL connection.
 * 
 * Example:
 * AT+CIPSTART=0,"TCP","192.168.3.60",3400
 * AT+CIPCLOSEMODE=0,1
 * AT+CIPCLOSE=0
 */
void tcpip_closeTCPIPMode(void) {
    
}


// AT+CIPCLOSE Closes TCP/UDP/SSL connection
/*
 * Commands
 * Set Command (used in multiple connections):
 * AT+CIPCLOSE=<link ID>
 * 
 * Function: close the TCP/UDP Connection.
 * 
 * Execute Command (used in multiple connections):
 * AT+CIPCLOSE
 * 
 * Response OK
 * 
 * Parameters
 * <link ID>: ID of the connection to be closed. When ID is 5, all connections 
 * will be closed. (In server mode, the ID 5 has no effect.)
 */
void tcpip_closeTCPIPConnection(void) {
}


// AT+CIPRECVMODE - Set TCP Receive Mode
/*
 * Set Command: AT+CIPRECVMODE=<mode>
 * 
 * Query Command: AT+CIPRECVMODE?
 * 
 * Response:
 * +CIPRECVMODE:<mode>
 * OK
 * OK
 * 
 * Parameters:
 * <mode>: the receive mode of TCP data is active mode by default.
 *  - 0: active mode - ESP8266 will send all the received TCP data instantly to 
 *       host MCU through UART with header ?+IPD".
 *  - 1: passive mode - ESP8266 will keep the received TCP data in an internal 
 *       buffer (default is 2920 bytes), and wait for host MCU to read the data. 
 *      If the buffer is full, the TCP transmission will be blocked.
 * 
 * Example AT+CIPRECVMODE=1
 * 
 * Note:
 * The configuration is for normal TCP transmission only, and cannot be used 
 * on SSL, UDP or WiFi-UART passthrough mode.
 * If the passive mode is enabled, when ESP8266 receives TCP data, it will 
 * prompt the following message in different scenarios:
 *  - for multiple connection mode (AT+CIPMUX=1), the message is: 
 *    +IPD,<link ID>,<len>
 *  - for single connection mode (AT+CIPMUX=0), the message is: 
 *    +IPD,<len>
 *  - <len> is the total length of TCP data in buffer
 */
void tcpip_setTCPReceiveMode(uint8_t mode) {
    if(mode == 0 || mode == 1) {
        printf("AT+CIPRECVMODE=%d\r\n", mode);
    }
    do { esp_waitForOK(); }while( server.busy );
}



// Funktion som læser input på det serielle input og returnerer pointer 
// til modtaget streng
/* 
 * Eksempel på header:
 * 0,CLOSED
 * 0,CONNECT
 * 1,CONNECT
 * 
 * +IPD,0,482:GET / HTTP/1.1
 * Host: 192.168.0.15:333
 * Connection: keep-alive
 * Cache-Control: max-age=0
 * Upgrade-Insecure-Requests: 1
 * User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML,
 * like Gecko) Chrome/76.0.3809.132 Safari/537.36
 * DNT: 1
 * Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/a
 * png,o/o;q=0.8,application/signed-exchange;v=b3
 * Accept-Encoding: gzip, deflate 
 * Accept-Language: da-DK,da;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6
 * 
 * Der er flere ting vi er interesseret i iforhold til GET request fra browser:
 * 
 * +IPD,0,482:GET / HTTP/1.1 <- Vores buffer størrelse
 * Host: 192.168.0.15:333 <- Hvad kommer REQUEST fra
 * Upgrade-Insecure-Requests: 1 <- Browser spørger efter en HTTPS forbindelse
 * Accept-Language: da-DK,da;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6 <- Hvis vi vil have sprog styring
 * 
 */
void tcpip_PassiveReceive(void) {
    // igennem linie for linie
    char     buffer[64];
    uint16_t count      = 0;
    uint16_t bufferSize = 0;
    uint16_t actualSize = 0;
    uint8_t line[80]; // Kan måske være mindre
    
    // Hent buffer størrelse fra ESP modulet
    // resolve
    // actualSize = resolvedSize;
    
    //+IPD
    
    // Hent data fra ESP tcpip buffer i chunks af 64 bytes
    if(server.clientConnected){
        //AT+CIPRECVDATA
        printf("AT+CIPRECVDATA=64\r\n"); // Get data for single connection mode
        
        do {
            /*uint8_t index = 0;

            while(index <= 64) {
                if(count <= actualSize)
                    break;
                
                // Tag de 64 tegn ind og afkod linie
                if (EUSART1_is_rx_ready()) {
                    char tmp = EUSART1_Read();
                    EUSART1_Write(tmp); // Echo over til terminal

                    if (isprint(tmp)) {
                        buffer[index] = tmp;
                        index++;
                        buffer[index] = 0x00;
                        if (index >= sizeof(a)) {
                            index--;
                        }
                    }

                    if (tmp == '\n' || tmp == '\r') {
                        index = 0;

                        // Compare the string for commands
                        esp_resolveUartLine();
                    }
                }
                index++;
                count++;
            }*/
        } while (count <= actualSize);
        
    } else if ( server.multibleConnections ) {
        for(uint8_t clients = 1; clients<=5; clients++) { // Get data for multible mode connection
            // Samme smøre
        }
    } else {
        char msg[] = "Kunne ikke hente data fra ESP buffer\r\n";
        //esp_terminalTransmit(*msg);
    }
}


//AT+SAVETRANSLINK Saves the transparent transmission link in the flash
/*
 *Save TCP Single Connection in Flash
 * Set Command:
 * AT+SAVETRANSLINK=<mode>,<remote IP or domain name>,<remote port>[,<type>,
 * <TCP keep alive>]
 * 
 * Response: OK
 * 
 * Parameters:
 * <mode>:
 *  - 0: ESP8266 will NOT enter UART-Wi-Fi passthrough mode on power-up.
 *  - 1: ESP8266 will enter UART-Wi-Fi passthrough mode on power-up.
 * <remote IP>: remote IP or domain name.
 * <remote port>: remote port.
 * [<type>] (optional): TCP or UDP, TCP by default.
 * [<TCP keep alive>] (optional): TCP is kept alive. This function is 
 *      disabled by default.
 *  - 0: disables the TCP keep-alive function.
 *  - 1 ~ 7200: keep-alive detection time interval; unit: second (s).
 * 
 * Notes:
 *  - This command will save the UART-Wi-Fi passthrough mode and its link in the 
 *    flash. ESP8266 will enter the UART-Wi-Fi passthrough mode on any subsequent 
 *    power cycles.
 *  - As long as the remote IP (or domain name) and port are valid, the 
 *    configuration will be saved in the flash.
 * 
 * Example: AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"
 * 
 * Save UDP Transmission in Flash
 * Set Command AT+SAVETRANSLINK=<mode>,<remote IP>,<remote port>,<type>
 * [,<UDP local port>]
 * 
 * Response: OK
Parameters
? <mode>:
? 0: normal mode; ESP8266 will NOT enter UART-Wi-Fi passthrough mode on power-up.
? 1: ESP8266 enters UART-Wi-Fi passthrough mode on power-up.
? <remote IP>: remote IP or domain name.
? <remote port>: remote port.
? [<type>] (optional): UDP; TCP by default.
? [<UDP local port>] (optional): local port when UDP transparent transmission is enabled on
power-up.
Notes
? This command will save the UART-Wi-Fi passthrough mode and its link in the flash. ESP8266 will
enter the UART-Wi-Fi passthrough mode on any subsequent power cycles.
? As long as the remote IP (or domain name) and port are valid, the configuration will be saved in
the user parameter area in the flash.
Example AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005
 * 
 * 
 */


// AT+CIPDINFO - Shows remote IP and remote port with +IPD
/*
 * Set Command AT+CIPDINFO=<mode>
 * 
 * Response: OK
 * 
 * Parameters:
 * <mode>:
 *  - 0: does not show the remote IP and port with +IPD.
 *  - 1: shows the remote IP and port with +IPD.
 * 
 * Example: AT+CIPDINFO=1
 */
void tcpip_showIPDRemoteIP(uint8_t mode) {
    printf("AT+CIPDINFO=%d\r\n", mode);
    do { esp_waitForOK(); } while( server.busy );
}


//+IPD ESP receives network data
/*
 * Single connection:
 * (+CIPMUX=0)+IPD,<len>[,<remote IP>,<remote port>]:<data>
 * 
 * multiple connections:
 * (+CIPMUX=1)+IPD,<link ID>,<len>[,<remote IP>,<remote port>]:<data>
 * 
 * Parameters
 * The command is valid in normal command mode. When the module receives network 
 * data, it will send the data through the serial port using the +IPD command.
 * [<remote IP>]: remote IP, enabled by command AT+CIPDINFO=1.
 * [<remote port>]: remote port, enabled by command AT+CIPDINFO=1.
 * <link ID>: ID number of connection.
 * <len>: data length.
 * <data>: data received.
 */


// AT+CIPSEND Sends data
/*
 * Set Command:
 * 1. Single connection: (+CIPMUX=0) AT+CIPSEND=<length>
 * 2. Multiple connections: (+CIPMUX=1) AT+CIPSEND=<link ID>,<length>
 * 3. Remote IP and ports can be set in UDP transmission:
 *    AT+CIPSEND=[<link ID>,]<length> [,<remote IP>,<remote port>]
 * Function: to configure the data length in normal transmission mode.
 * 
 * Execute Command:
 * AT+CIPSEND
 * Function: to start sending data in transparent transmission mode.
 * 
 * Response:
 * Send data of designated length.
 * Wrap return > after the Set Command. Begin receiving serial data. When 
 * data length defined by <length> is met, the transmission of data starts.
 * If the connection cannot be established or gets disrupted during data 
 * transmission, the system returns:
 * ERROR
 * 
 * If data is transmitted successfully, the system returns:
 * SEND OK
 * 
 * If it failed, the system returns:
 * SEND FAIL
 * 
 * Wrap return > after executing this command. Enter transparent transmission, 
 * with a 20 ms interval between each packet, and a maximum of 2048 bytes per 
 * packet. When a single packet containing +++ is received, ESP8266 returns to 
 * normal command mode.
 * 
 * Please wait for at least one second before sending the next AT command.
 * 
 * This command can only be used in transparent transmission mode which requires 
 * single connection.
 * 
 * For UDP transparent transmission, the value of <UDP mode> has to be 0 when 
 * using AT+CIPSTART.
 * 
 * Parameters:
 * <link ID>: ID of the connection (0~4), for multiple connections.
 * <length>: data length, MAX: 2048 bytes.
 * [<remote IP>]: remote IP can be set in UDP transmission.
 * [<remote port>]: remote port can be set in UDP transmission.
 * 
 * For more information please see: ESP8266 AT Command Examples.
 */


// AT+CIPSENDEX Sends data when length of data is <length>, or when \0 appears in the data
/*
 * Set Command
 * 1. Single connection: (+CIPMUX=0) AT+CIPSENDEX=<length>
 * 2. Multiple connections: (+CIPMUX=1) AT+CIPSENDEX=<link ID>,<length>
 * 3. Remote IP and ports can be set in UDP transmission:
 * AT+CIPSENDEX=[<link ID>,]<length>[,<remote IP>,<remote port>]
 * 
 * Function: to configure the data length in normal transmission mode.
 * 
 * Response:
 * Send data of designated length. Wrap return > after the Set Command. Begin 
 * receiving serial data. When the requirement of data length, determined by 
 * <length>, is met, or when \0 appears in the data, the transmission starts.
 * If connection cannot be established or gets disconnected during transmission, 
 * the system returns:
 * ERROR
 *    If data are successfully transmitted, the system returns:
 * SEND OK
 *    If it failed, the system returns:
 * SEND FAIL
 * 
 * Parameters:
 * <link ID>: ID of the connection (0~4), for multiple connections.
 * <length>: data length, MAX: 2048 bytes.
 * 
 * When the requirement of data length, determined by <length>, is met, or when 
 * \0 appears, the transmission of data starts. Go back to the normal command 
 * mode and wait for the next AT command.
 * When sending \0, please send it as \\0.
 */


// AT+CIPSENDBUF Writes data into TCP-send-buffer
/*
 * Set Command:
 * 1. Single connection: (+CIPMUX=0) AT+CIPSENDBUF=<length>
 * 2. Multiple connections: (+CIPMUX=1) AT+CIPSENDBUF=<link ID>,<length>
 * 
 * Response:
 * <current segment ID>,<segment ID of which sent successfully>
 * OK
 * >
 * 
 * Wrap return > begins receiving serial data; when the length of data defined 
 * by the parameter <length> is met, the data is sent; if the data length over 
 * the value of <length>, the data will be discarded, and the command returns busy.
 * 
 * If the connection cannot be established, or if it is not a TCP connection, or 
 * if the buffer is full, or some other error occurs, the command returns
 * ERROR
? If data is transmitted successfully,
? for single connection, the response is:
<segment ID>,SEND OK
? for multiple connections, the response is:
<link ID>,<segment ID>,SEND OK
? If it failed, the system returns:
SEND FAIL
Parameters
? <link ID>: ID of the connection (0~4), for multiple connections.
? <segment ID>: uint32; the ID assigned to each data packet, starting from 1; the ID number increases
by 1 every time a data packet is written into the buffer.
? <length>: data length; MAX: 2048 bytes.
Notes
? This command only writes data into the TCP-send-buffer, so it can be called continually, and the user
need not wait for SEND OK; if a TCP segment is sent successfully, it will return <segment ID>,SEND OK.
? Before data length reaches the value defined by <length>, input +++ can switch back from data mode
to command mode, and discard the data received before.
? This command can NOT be used for SSL connections.
 */


// AT+CIPBUFRESET - Resets the segment ID count
/*
 * Set Command:
 * 1. Single connection: (+CIPMUX=0) AT+CIPBUFRESET
 * 2. Multiple connections: (+CIPMUX=1) AT+CIPBUFRESET=<link ID>
 * 
 * Response:
 * OK
 * 
 * If the connection is not established or there is still TCP data waiting to be 
 * sent, the response will be:
 * ERROR
 * Parameter <link ID>: ID of the connection (0~4), for multiple connections.
 * 
 * Note:
 * This command can only be used when AT+CIPSENDBUF is used.
 */


// AT+CIPBUFSTATUS - Checks the status of TCP-send-buffer
/*
 * Set Command
 * 1. Single connection: (+CIPMUX=0) AT+CIPBUFSTATUS
 * 2. Multiple connections: (+CIPMUX=1) AT+CIPBUFSTATUS=<link ID>
 * 
 * Response:
 * <next segment ID>,<segment ID sent >,<segment ID successfully sent>,
 * <remain buffer size>,<queue number>
 * OK
 * 
 * Parameters:
 * <next segment ID>: the next segment ID obtained by AT+CIPSENDBUF;
 * <segment ID sent>: the ID of the TCP segment last sent;
 * Only when <next segment ID> - <segment ID sent> = 1, can AT+CIPBUFRESET be 
 * called to reset the counting.
 * <segment ID successfully sent>: the ID of the last successfully sent TCP 
 * segment;
 * <remain buffer size>: the remaining size of the TCP-send-buffer;
 * <queue number>: available TCP queue number; it?s not reliable and should be 
 * used as a reference only.
 * 
 * Notes: This command can not be used for SSL connection.
 * 
 * Example:
 * For example, in single connection, the command AT+CIPBUFSTATUS returns:
 * 20,15,10,200,7
 * 
 * Description:
 * 20: means that the latest segment ID is 19; so when calling AT+CIPSENDBUF the 
 *     next time, the segment ID returned is 20;
 * 15: means that the TCP segment with the ID 15 is the last segment sent, but 
 *     the segment may not be successfully sent;
 * 10: means that the TCP segment with the ID 10 was sent successfully;
 * 200: means that the remaining size of the TCP-send-buffer is 200 bytes;
 * 7: the available TCP queue number; it is not reliable and should be used as 
 *    a reference only;
 * when the queue number is 0, no TCP data can be sent.
 */


// AT+CIPCHECKSEQ - Checks if a specific segment is sent or not
/*
 * Set Command
 * 1. Single connection: (+CIPMUX=0) AT+CIPCHECKSEQ=<segment ID>
 * 2. multiple connections: (+CIPMUX=1) AT+CIPCHECKSEQ=<link ID>,<segment ID>
 * 
 * Response [<link ID>,]<segment ID>,<status>
 * OK
 * 
 * Parameters:
 * ? The command can only be used to record the status of the last 32 segments at most.
 * ? [<link ID>]: ID of the connection (0~4), for multiple connection;
 * ? <segment ID>: the segment ID obtained by calling AT+CIPSENDBUF;
 * ? <status>:
 *   FALSE: the segment-sending failed;
 *   TRUE: the segment was sent successfully.
 * 
 * Notes: This command can only be used when AT+CIPSENDBUF is used.
 */



// AT+CIFSR Gets the local IP address
/*
 * Execute Command: AT+CIFSR
 * 
 * Response:
 * +CIFSR:APIP,<SoftAP IP address>
 * +CIFSR:APMAC,<SoftAP MAC address>
 * +CIFSR:STAIP,<Station IP address>
 * +CIFSR:STAMAC,<Station MAC address>
 * OK
 * 
 * Parameters:
 * <IP address>:
 * IP address of the ESP8266 SoftAP;
 * IP address of the ESP8266 Station.
 * <MAC address>:
 * MAC address of the ESP8266 SoftAP;
 * MAC address of the ESP8266 Station.
 * 
 * Notes:
 * Only when the ESP8266 Station is connected to an AP can the Station IP can be 
 * queried.
 */
void tcpip_getLocalIP(void) {
    printf("AT+CIFSR\r\n"); // Resolve first line
    //esp_uartReceive();
}


void tcpip_getStationMac(void) {
    printf("AT+CIFSR\r\n"); // Resolve second line
    //esp_uartReceive();
}


void tcpip_getSoftAPIP(void) {
    printf("AT+CIFSR\r\n"); // Resolve third line
    //esp_uartReceive();
}


void tcpip_getSoftAPMac(void) {
    printf("AT+CIFSR\r\n"); // Resolve fourth line
    //esp_uartReceive();
}


void tcpip_getStationGateway(void) {
    printf("AT_CWDHCPS?;"); // Resolve second line
    //esp_uartReceive();
}


void tcpip_getStationNetmask(void) {
    printf("AT_CWDHCPS?;");  // Resolve third line
    //esp_uartReceive();
}


// AT+CIPSTO Sets timeout when ESP8266 runs as TCP server
/*
 * Query Command: AT+CIPSTO?
 * Function: to check the TCP server timeout.
 * 
 * Set Command: AT+CIPSTO=<time>
 * Function: to set the TCP server timeout.
 * 
 * Response:
 * +CIPSTO:<time>
 * OK
 * OK
 * 
 * Parameter <time>: TCP server timeout within the range of 0 ~ 7200s.
 * 
 * Notes:
 *  - ESP8266 configured as a TCP server will disconnect from the TCP client 
 *    that does not communicate with it until timeout.
 *  - If AT+CIPSTO=0, the connection will never time out. This configuration 
 *    is not recommended.
 * 
 * Example:
 * AT+CIPMUX=1
 * AT+CIPSERVER=1,1001
 * AT+CIPSTO=10
 */


// AT+PING Ping packets
/*
 * Set Command: AT+PING=<IP>
 * Function: Ping packets.
 * 
 * Response:
 * +<time>
 * OK
 *    or
 * +timeout
 * ERROR
 * 
 * Parameters:
 * <IP>: string; host IP or domain name
 * <time>: the response time of ping
 * 
 * Examples:
 * AT+PING="192.168.1.1"
 * AT+PING="www.baidu.com"
 */
void tcpip_checkInternet(void) {
    printf("AT+PING=\"https://www.google.com/\"\r\n");
    if( esp_waitForOK() ) {
        server.internet = 1;
    } else {
        server.internet = 0;
    }
}


/**************************************************************/
#endif






/**************************************************************
                       TCP/IP Client
 **************************************************************/
#ifndef TCPIPCLIENT_H


// AT+CIPSTART Establishes TCP connection, UDP transmission or SSL connection
/*
 * Establish TCP Connection
 * 
 * Set Command Single TCP connection (AT+CIPMUX=0):
 * AT+CIPSTART=<type>,<remote IP>,<remote port>[,<TCP keep alive>]
 * 
 * Multiple TCP Connections (AT+CIPMUX=1):
 * AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,<TCP keep alive>]
 * 
 * Response:
 * OK
 *    or
 * ERROR
 *    or
 * if the TCP connection is already established, the response is:
 * ALREADY CONNECTED
 * 
 * Parameters:
 * <link ID>: ID of network connection (0~4), used for multiple connections.
 * <type>: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
 * <remote IP>: string parameter indicating the remote IP address.
 * <remote port>: the remote port number.
 * [<TCP keep alive>]: detection time interval when TCP is kept alive; this 
 *    function is disabled by default.
 *  - 0: disable TCP keep-alive.
 *  - 1 ~ 7200: detection time interval; unit: second (s).
 * 
 * Examples:
 * AT+CIPSTART="TCP","iot.espressif.cn",8000
 * AT+CIPSTART="TCP","192.168.101.110",1000
 * 
 * For more information please see: ESP8266 AT Command Examples.
 */

/* Establish UDP Transmission
 * Set Command Single connection (AT+CIPMUX=0):
 * AT+CIPSTART=<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)]
 * 
 * Multiple connections (AT+CIPMUX=1):
 * AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)]
 * 
 * Response:
 * OK
 *    or
 * ERROR
 *    or if the UDP transmission is already established, the response is:
 * ALREADY CONNECTED
 * 
 * Parameters:
 * <link ID>: ID of network connection (0~4), used for multiple connections.
 * <type>: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
 * <remote IP>: string parameter indicating the remote IP address.
 * <remote port>: remote port number.
 * [<UDP local port>]: optional; UDP port of ESP8266.
 * [<UDP mode>]: optional. In the UDP transparent transmission, the value of this parameter has to be 0.
 *  - 0: the destination peer entity of UDP will not change; this is the default setting.
 *  - 1: the destination peer entity of UDP can change once.
 *  - 2: the destination peer entity of UDP is allowed to change.
 * 
 * ? Notice:
 * To use <UDP mode> , <UDP local port> must be set first.
 * 
 * Example: AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
 * 
 * For more information please see: ESP8266 AT Command Examples.
 */

/* Establish SSL Connection
 * Set Command AT+CIPSTART=[<link ID>,]<type>,<remote IP>,<remote port>[,<TCP keep alive>]
 * 
 * Response:
 * OK
 *    or
 * ERROR
 *    if the TCP connection is already established, the response is:
 * ALREADY CONNECTED
 * 
 * Parameters:
 * <link ID>: ID of network connection (0~4), used for multiple connections.
 * <type>: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
 * <remote IP>: string parameter indicating the remote IP address.
 * <remote port>: the remote port number.
 * [<TCP keep alive>]: detection time interval when TCP is kept alive; this function is disabled by default.
 *  - 0: disable the TCP keep-alive function.
 *  - 1 ~ 7200: detection time interval, unit: second (s).
 * 
 * Notes:
 * ESP8266 can only set one SSL connection at most.
 * SSL connection does not support UART-Wi-Fi passthrough mode (transparent transmission).
 * SSL connection needs a large amount of memory; otherwise, it may cause 
 * system reboot. The command AT+CIPSSLSIZE=<size> can be used to enlarge the 
 * SSL buffer size.
 * 
 * Example: AT+CIPSTART="SSL","iot.espressif.cn",8443
 */


#endif /*TCPIPCLIENT_H*/






/************************************************************
                        Serverpages
 ************************************************************/
#ifndef SERVERPAGES_H

void tcpip_serverPage(void) {
    char msg[16];
    
    uint8_t client=1;
    
    if( server.clientConnected && server.clientAvailable) {
        lcd_write("@Ny klient", 0, 0);
        
        //uint8_t currentTime = millis();
        //uint8_t previousTime = currentTime;
        
        while ( server.clientConnected ) { // loop while the client's connected
            //currentTime = millis();
            
            // Hvis klienten er forbundet, send sider
            if ( server.clientAvailable ) {             // if there's bytes to read from the client,
                tcpip_sendHTTPHeader();
                //i2c_eeprom_dumpLogData();
                tcpip_sendHTTPPage();
            }
            
            // End the TCP connection.
            printf("AT+CIPCLOSE=0\r\n"); //Response: 0,CLOSED
            break;
        }
    }

    sprintf(msg, "@Klient afbrudt");
    lcd_write(msg, 0, 0);
    printf("");
}


void tcpip_sendHTTPHeader(void) {
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    const char headerMsg[] = { "HTTP/1.1 200 OK\nContent-type:text/html\nConnection: close\n\n" };
    printf("AT+CIPSEND=0,%d\r\n", strlen(headerMsg) ); // Starter
    __delay_ms(70);
    printf(headerMsg);
    __delay_ms(70);
}


struct GPIOState {
    uint8_t output4State;
    uint8_t output5State;
};

typedef struct GPIOState gpiostate_t; // Angiver type definintion

gpiostate_t gpio;

void tcpip_sendHTTPPage(void) {
    char tmp[60];
    unsigned int len = 0;
    
    len += strlen(htmlMsgPart1);

    // Display current state, and ON/OFF buttons for GPIO 5
    /*
     * const char htmlMsgGPOI1[25];
    sprintf(tmp, "<p>GPIO 5 - State %s</p>\n", gpio.output5State);
    strcpy( htmlMsgGPOI1, tmp);
    len += strlen(tmp);
     */

    
    // If the output5State is off, it displays the ON button       
    //if (gpio.output5State==0) {
    //    strcat(htmlMsg, "<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
    //} else {
    //    strcat(htmlMsg, "<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
    //}
    
    // Display current state, and ON/OFF buttons for GPIO 4  
    //sprintf(tmp, "<p>GPIO 4 - State %s</p>", gpio.output4State);
    //strcat(htmlMsg, tmp);
    
    // If the output4State is off, it displays the ON button       
    //if (gpio.output4State==0) {
    //    strcat(htmlMsg, "<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
    //} else {
    //    strcat(htmlMsg, "<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
    //}
    
    const char htmlMsgPart2[] = {  "</body></html>\n\n" };
    len += strlen(htmlMsgPart2);
    
    printf("AT+CIPSEND=0,%d\r\n", len );
    __delay_ms(70);
    printf( htmlMsgPart1 );
    //printf( htmlMsgGPOI1 );
    printf( htmlMsgPart2 );
    __delay_ms(200);
}


void toogleGPIO(uint8_t ioPin, uint8_t state) {
    
}

/************************************************************/
#endif











/************************************************************
                               DNS
 ************************************************************/

#ifndef DNS_C


// AT+CIPDNS - Sets user-defined DNS servers
/*
 * Query Command: AT+CIPDNS?
 * Function: Get the current DNS server.
 * 
 * Set Command: AT+CIPDNS_CUR=<enable>[,<DNS server0>,<DNS server1>]
 * Function: Set user-defined DNS servers.
 * 
 * Response:
 * [+CIPDNS_CUR:<DNS server0>]
 * [+CIPDNS_CUR:<DNS server1>]
 * OK
 * OK
 * 
 * Parameters:
 * <enable>:
 *  - 0: disable to use user-defined DNS servers;
 *  - 1: enable to use user-defined DNS servers.
 * <DNS server0>: optional parameter indicating the first DNS server;
 * <DNS server1>: optional parameter indicating the second DNS serve.
 * 
 * Example:
 * AT+CIPDNS_CUR=1,"208.67.220.220"
 * 
 * Note:
 * For command: AT+CIPDNS_CUR=0 (disable to use user-defined DNS servers), 
 * "208.67.222.222" will be used as DNS server by default. And the DNS server 
 * may change according to the configuration of the router which the chip 
 * connected to.
 * 
 * For command: AT+CIPDNS_CUR=1 (enable to use user-defined DNS servers, but 
 * the <DNS server> parameters are not set), servers "208.67.222.222" will be 
 * used as DNS server by default.
 * 
? <DNS server0> and <DNS server1> cannot be set to the same server.
 * 
 * Versions:
 * AT+CIPDNS
 * AT+CIPDNS_CUR
 * AT+CIPDNS_DEF
 */
void setDNSServers(void) {
}


// AT+CIPDOMAIN DNS function
/*
 * Execute Command AT+CIPDOMAIN=<domain name>
 * 
 * Response_
 * +CIPDOMAIN:<IP address>
 * OK
 *     or
 * DNS Fail
 * ERROR
 * 
 * Parameter:
 * <domain name>: the domain name, length should be less than 64 bytes.
 * 
 * Example:
 * AT+CWMODE=1 // set Station mode
 * AT+CWJAP="SSID","password" // access to the internet
 * AT+CIPDOMAIN="iot.espressif.cn" // DNS function
 */
void setDNSDomain(void) {
    printf("AT+CIPDOMAIN=\"iot.karsten.esp\"\r\n");
    esp_waitForOK();
}


// AT+MDNS - Configures the MDNS Function
/*
 * Set Command:
 * AT+MDNS=<enable>,<hostname>,<server_name>,<server_port>
 * 
 * Response:
 * OK
 *    or opmode mismatch when mdns
 * ERROR
 * 
 * Parameters:
 * <enable>:
 *  - 1: enables the MDNS function; the following three parameters need to be set.
 *  - 0: disables the MDNS function; the following three parameters need not to be set.
 * <hostname>: MDNS host name
 * <server_name>: MDNS server name
 * <server_port>: MDNS server port
 * 
 * Notes:
 * Please do not use special characters (such as .) or a protocol name 
 * (for example, http) for <hostname> and <server_name>.
 * ESP8266 SoftAP mode does not support the MDNS function for now.
 * 
 * Example:
 * AT+MDNS=1,"espressif","iot",8080
 */
void wifi_mDNSConfig(void) {
    printf("AT+MDNS=1,\"espressif\",\"iot\",8080");
}

/************************************************************/
#endif /*DNS_C*/



/************************************************************
                               DHCP
 ************************************************************/
#ifndef DHCH_C

// AT+CWDHCP - Enables/Disables DHCP
/*
 * Query Command: AT+CWDHCP?
 * 
 * Set Command:
 * AT+CWDHCP=<<mode>,<en>
 * Function: to enable/disable DHCP.
 * 
 * Response:
 * DHCP disabled or enabled now? OK
 * 
 * Parameters:
 * Bit0:
 *  - 0: SoftAP DHCP is disabled.
 *  - 1: SoftAP DHCP is enabled.
 * Bit1:
 *  - 0: Station DHCP is disabled.
 *  - 1: Station DHCP is enabled.
 * <mode>:
 *  - 0: Sets ESP8266 SoftAP
 *  - 1: Sets ESP8266 Station
 *  - 2: Sets both SoftAP and Station
 * <en>:
 *  - 0: Disables DHCP
 *  - 1: Enables DHCP
 * 
 * Notes:
 * The configuration changes will not be saved in flash.
 * This Set Command interacts with static-IP-related AT commands 
 * (AT+CIPSTA-related and AT+CIPA-related commands):
 *  - If DHCP is enabled, static IP will be disabled;
 *  - If static IP is enabled, DHCP will be disabled;
 *  - Whether it is DHCP or static IP that is enabled depends on the last 
 *    configuration.
 * 
 * Example:
 * AT+CWDHCP=0,1
 
 * Versions:
 * AT_CWDHCP
 * AT_CWDHCP_CUR
 * AT_CWDHCP_DEF
 */
#define DHCP_MODE = 2
#define DHCP_ENABLE = 1

void setupDHCP(void) {
	
}


// AT+CWDHCPS - Sets the IP Address Allocated by ESP8266 SoftAP DHCP;
/*
 * Query Command: AT+CWDHCPS_CUR?
 * 
 * Set Command: AT+CWDHCPS_CUR=<enable>,<leasetime>,<start IP>,<end IP>
 * Function: sets the IP address range of the ESP8266 SoftAP DHCP server.
 * 
 * Response:
 * +CWDHCPS_CUR=<lease time>,<start IP>,<end IP>
 * 
 * OK
 * 
 * Parameters
 * <enable>:
 *  - 0: Disable the settings and use the default IP range.
 *  - 1: Enable setting the IP range, and the parameters below have to be set.
 * <lease time>: lease time; unit: minute; range [1, 2880].
 * <start IP>: start IP of the IP range that can be obtained from ESP8266 SoftAP 
 *    DHCP server.
 * <end IP>: end IP of the IP range that can be obtained from ESP8266 SoftAP 
 *    DHCP server.
 * 
 * Notes:
 * The configuration changes will NOT  be saved in the flash.
 * 
 * This AT command is enabled when ESP8266 runs as SoftAP, and when DHCP is 
 * enabled. The IP address should be in the same network segment as the IP 
 * address of ESP8266 SoftAP.
 * 
 * Examples:
 * AT+CWDHCPS_CUR=1,3,"192.168.4.10","192.168.4.15"
 *    or
 * AT+CWDHCPS_CUR=0 //Disable the settings and use the default IP range.
 * 
 * Versions:
 * AT+CWDHCPS
 * AT+CWDHCPS_CUR
 * AT+CWDHCPS_DEF
 */
#define DHCP_ENABLE    = 1
#define DHCP_LEASETIME = 3
#define DHCP_START_IP  = "192.168.4.10"
#define DHCP_END_IP    = "192.168.4.15"


//#define AT_CWDHCPS_DEF { printf("AT+CWDHCPS_CUR=<enable>,<leasetime>,<start IP>,<end IP>"); }
//#define AT_CWDHCPS_CUR_DEFAULT { printf("AT+CWDHCPS_CUR=0"); }
//#define AT_CWDHCPS_CUR_QUERY { printf("AT+CWDHCPS_CUR=0"); }       
void esp_setSoftAPDHCPIP(uint8_t *startIP, uint8_t *endIP, uint8_t enable, uint8_t leasetime ) {
    printf("AT+CWDHCPS=%d,%d,\"%s\",\"%s\"\r\n", enable, leasetime, startIP, endIP);
}

#endif




/************************************************************
                            SNTP
 ************************************************************/
#ifndef SNTP_C


// AT+CIPSNTPCFG?Sets the Configuration of SNTP
/* 
 * Query Command: AT+CIPSNTPCFG?
 * 
 * Set Command:
 * AT+CIPSNTPCFG=<enable>[,<timezone>][,<SNTP server0>,<SNTP server1>,<SNTP server2>]
 * 
 * Response
 * +CIPSNTPCFG:<enable>,<timezone>,<SNTP server1>[,<SNTP server2>,<SNTP server3>]
 * OK
 * OK
 * 
 * Parameters:
 * <enable>:
 *  - 0: SNTP is disabled;
 *  - 1: SNTP is enabled.
 * <timezone>: time zone; range: [-11,13]; if SNTP is enabled, the <timezone> has to be set;
 * <SNTP server0>: optional parameter indicating the first SNTP server;
 * <SNTP server1>: optional parameter indicating the second SNTP server;
 * <SNTP server2>: optional parameter indicating the third SNTP server.
 * 
 * Example:
 * AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn","us.pool.ntp.org"
 * 
 * Note:
 * If the <SNTP server> parameters are not set, servers "cn.ntp.org.cn",
 * "ntp.sjtu.edu.cn","us.pool.ntp.org" will be used by default.
 */
void sntp_sntpInit(void) {
    printf("AT+CIPSNTPCFG=1,2,\"dk.pool.ntp.org\"\r\n");  // enable, Danmark GMT+2
    esp_waitForOK();
}

// AT+CIPSNTPTIME?Checks the SNTP Time
/* 
 * Query Command AT+CIPSNTPTIME?
 * 
 * Response +CIPSNTPTIME:<time>
 * OK
 * 
 * Parameters:
 * <time>: SNTP time
 * 
 * Example:
 * +CIPSNTPTIME:Thu Aug 04 14:48:05 2016
 * OK
 * 
 * Example:
 * AT+CWMODE=1 //set station mode
 * AT+CWJAP="DemoAP","password" //connect to router, access the internet
 * AT+CIPSNTPCFG=1,8 //set time zone
 * AT+CIPSNTPTIME? //get time
 */
void sntp_checkTime(void) {
    if( server.internet ) {
        printf("AT+CIPSNTPTIME?\r\n");
        esp_waitForOK();
    }
}


/************************************************************/
#endif