/**************************************************************
                       Temperatur sensor
 **************************************************************/

#ifndef SENSOR_H
#include "sensor.h"
#endif


void sensor_testTempSensor(void) {
    i2c_write_serial(temperature_addr, 0, 0);
    i2c_read_serial(temperature_addr, data_out, 4);
}


void sensor_getReading(void) {
    uint32_t tmp_moist = 0;
    int32_t  tmp_temp  = 0;
    uint8_t  length    = 0;

    //Læs fra sensor
    i2c_write_serial(temperature_addr, 0, 0);
    i2c_read_serial(temperature_addr, data_out, 4);
   
    tmp_moist  = ( data_out[0] & 0b00111111 ) << 8;
    tmp_moist += data_out[1];
    tmp_moist *= 100;
    tmp_moist /= 16382;
    
    sensor_updateHumidity( (int8_t) tmp_moist);
    
    // Sensor er gyldig mellem -25 til 85 grader
    tmp_temp  = data_out[2] << 8;
    tmp_temp += data_out[3];
    tmp_temp >>= 2;
        
    tmp_temp *= 16500;
    tmp_temp /= 16382;
    tmp_temp -= 4000;

    // Indsætter start kommando: skriver tegn
    tmp_string[length++] = 0x40;
    
    // Vi tjekker om tallet er negativt
    if(tmp_temp < 0) {
        tmp_string[length++] = 0x2D;
        tmp_temp *= -1;
    }
    
    // Måler på sidste ciffer 00,0x
    int8_t decimal2 = tmp_temp % 10;
    tmp_temp /= 10;

    // Måler på 2. sidste ciffer 00,x
    int8_t decimal1 = tmp_temp % 10;
    
    if(decimal2 >= 5 ) {
        decimal1++;
        tmp_temp++;
    }
    
    tmp_temp /= 10;
    
    // Formatterer heltallet
    if(tmp_temp > 9) {
        uint8_t ones = (uint8_t) tmp_temp % 10;
        tmp_temp /= 10;
        uint8_t tens = (uint8_t) tmp_temp;
        
        tmp_string[length++] = tens | 0b00110000; // Laver tallet til ASCII
        tmp_string[length++] = ones | 0b00110000; // --"--
    } else {
        tmp_string[length++] = tmp_temp;
    }
    
    // Formatterer komma tal
    tmp_string[length++] = 0x2C; //Komma tegn
    tmp_string[length++] = decimal1 | 0b00110000; // Laver tallet til ASCII;
    
    // Sætter gradtegn på
    //tmp_string[length++] = 0xDF; // Eksisterende grad tegn
    tmp_string[length++] = 0x00;
    /*tmp_string[length++] = 0x01;
    tmp_string[length++] = 0x02;
    tmp_string[length++] = 0x03;
    tmp_string[length++] = 0x04;
    tmp_string[length++] = 0x05;
    tmp_string[length++] = 0x06;
    tmp_string[length++] = 0x07;*/

    i2c_write_serial(display_addr, (char*) temp_pos, 2);
    i2c_write_serial(display_addr, tmp_string, length);
}


void sensor_updateTemperature(int8_t temp) {
    if(temp < 0) {
        i2c_write_serial(display_addr, (char*) temp_pos, 2);
        i2c_write_serial(display_addr, (char*) minus, 2);
        temp *= -1;
    } else if (temp > 9) {
        uint8_t tens = (uint8_t) (temp / 10) | 0b00110000;
        uint8_t ones = (uint8_t) (temp % 10) | 0b00110000;        
        char    output[] = {0x40, tens , ones, 3};
        
        i2c_write_serial(display_addr, (char*) temp_pos, 2);
        i2c_write_serial(display_addr, output, 3);
    }
}


void sensor_updateHumidity(uint8_t moist) {
    char output[20];
    output[0] = 0x40;
    
    if(moist >= 100) {
        output[1] = 1;
        output[1] |= 0b00110000;
        moist -= 100;
    } else {
        output[1] = 0b10100000;
    }
    
    if(moist >= 10) {
        output[2] = (moist / 10) | 0b00110000;
        output[3] = (moist % 10) | 0b00110000;
    } else {
        output[1] = 0b10100000;
        output[2] = 0b10100000;
        output[3] = moist | 0b00110000;
    }

    i2c_write_serial(display_addr, (char*) moist_pos, 2);
    i2c_write_serial(display_addr, output, 4);
}


/**************************************************************/

/**
 End of File
*/