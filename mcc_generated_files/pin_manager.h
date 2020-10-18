/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F26K22
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set led_error aliases
#define led_error_TRIS                 TRISAbits.TRISA0
#define led_error_LAT                  LATAbits.LATA0
#define led_error_PORT                 PORTAbits.RA0
#define led_error_ANS                  ANSELAbits.ANSA0
#define led_error_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define led_error_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define led_error_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define led_error_GetValue()           PORTAbits.RA0
#define led_error_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define led_error_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define led_error_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define led_error_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set led_ok aliases
#define led_ok_TRIS                 TRISAbits.TRISA1
#define led_ok_LAT                  LATAbits.LATA1
#define led_ok_PORT                 PORTAbits.RA1
#define led_ok_ANS                  ANSELAbits.ANSA1
#define led_ok_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define led_ok_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define led_ok_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define led_ok_GetValue()           PORTAbits.RA1
#define led_ok_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define led_ok_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define led_ok_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define led_ok_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set led_ready aliases
#define led_ready_TRIS                 TRISAbits.TRISA2
#define led_ready_LAT                  LATAbits.LATA2
#define led_ready_PORT                 PORTAbits.RA2
#define led_ready_ANS                  ANSELAbits.ANSA2
#define led_ready_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define led_ready_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define led_ready_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define led_ready_GetValue()           PORTAbits.RA2
#define led_ready_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define led_ready_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define led_ready_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define led_ready_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set led_client_connected aliases
#define led_client_connected_TRIS                 TRISAbits.TRISA3
#define led_client_connected_LAT                  LATAbits.LATA3
#define led_client_connected_PORT                 PORTAbits.RA3
#define led_client_connected_ANS                  ANSELAbits.ANSA3
#define led_client_connected_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define led_client_connected_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define led_client_connected_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define led_client_connected_GetValue()           PORTAbits.RA3
#define led_client_connected_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define led_client_connected_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define led_client_connected_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define led_client_connected_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set led_getrequest aliases
#define led_getrequest_TRIS                 TRISAbits.TRISA4
#define led_getrequest_LAT                  LATAbits.LATA4
#define led_getrequest_PORT                 PORTAbits.RA4
#define led_getrequest_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define led_getrequest_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define led_getrequest_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define led_getrequest_GetValue()           PORTAbits.RA4
#define led_getrequest_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define led_getrequest_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)

// get/set led_ap_connected aliases
#define led_ap_connected_TRIS                 TRISAbits.TRISA5
#define led_ap_connected_LAT                  LATAbits.LATA5
#define led_ap_connected_PORT                 PORTAbits.RA5
#define led_ap_connected_ANS                  ANSELAbits.ANSA5
#define led_ap_connected_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define led_ap_connected_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define led_ap_connected_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define led_ap_connected_GetValue()           PORTAbits.RA5
#define led_ap_connected_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define led_ap_connected_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define led_ap_connected_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define led_ap_connected_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set led_softap_connected aliases
#define led_softap_connected_TRIS                 TRISAbits.TRISA6
#define led_softap_connected_LAT                  LATAbits.LATA6
#define led_softap_connected_PORT                 PORTAbits.RA6
#define led_softap_connected_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define led_softap_connected_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define led_softap_connected_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define led_softap_connected_GetValue()           PORTAbits.RA6
#define led_softap_connected_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define led_softap_connected_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set led_tcp_server aliases
#define led_tcp_server_TRIS                 TRISAbits.TRISA7
#define led_tcp_server_LAT                  LATAbits.LATA7
#define led_tcp_server_PORT                 PORTAbits.RA7
#define led_tcp_server_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define led_tcp_server_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define led_tcp_server_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define led_tcp_server_GetValue()           PORTAbits.RA7
#define led_tcp_server_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define led_tcp_server_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/