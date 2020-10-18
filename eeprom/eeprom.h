/**********************************************************//**
 * @file:    eeprom.h
 * @author:  Karsten 'Kaki' Reitan S�rensen
 * @created: September 8, 2019, 10:02 PM
 * @brief:   EEPROM styring
 **************************************************************/
#pragma once

#ifndef MCC_H
    #include "../mcc_generated_files/mcc.h"
#endif

#include <string.h>
#include <ctype.h>

#ifndef I2C_H
#include "../i2c/i2c.h"
#endif

#ifndef EEPROM_H
#define	EEPROM_H


const uint8_t eeprom_addr      = 0b1010000; // Control byte 1010 000
const char    eeprom_test[]    = {0x00, 0x00, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
const char    eeprom_init[]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
const char    eeprom_pointer[] = {0x00, 0x00}; // Sætter til adresse 0x0000 standard. Kan lave om i software
uint8_t       eeprom_buffer[128];


const char eeprom_htmlTemplate[]   = "<h1>Temperatur og luftfugtighed</h1><div style='width: 100%;'><canvas id='canvas'></canvas></div>";
const char eeprom_fontTemplate[]   = "";
const char eeprom_cssTemplate[]    = "<style>canvas {-moz-user-select: none;-webkit-user-select: none;-ms-user-select: none;}</style>";
const char eeprom_scriptTemplate[] = "<script src='/node_modules/chart.js/dist/Chart.bundle.js'></script><script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script><script>var data=[],labels=[],	temperature=[], humidity=[];$.get('GetTestData.php', function(dataGet) {data=JSON.parse(dataGet);data['labels'].forEach(function(singleResult){labels.push(singleResult);});data['temperature'].forEach(function(singleResult){temperature.push(singleResult);});data['humidity'].forEach(function(singleResult){humidity.push(singleResult);});var ctx=document.getElementById('canvas').getContext('2d');window.myLine=new Chart(ctx,config);});var randomScalingFactor=function(){ return Math.round(Math.random()*100); };var randomColorFactor=function(){ return Math.round(Math.random()*255); };var randomColor=function(opacity){return'rgba('+randomColorFactor()+','+randomColorFactor()+','+randomColorFactor()+','+(opacity||'.9')+')'; };varconfig={type:'line',data:{labels:labels,datasets:[{ label:'Temperatur',data:temperature,fill:false },{ data:humidity,fill:false}]},options:{responsive:true,title:{display:true,text:'Temperatur og Luftfugtighed'},tooltips:{mode:'label'},hover:{mode:'dataset'},scales:{xAxes:[{display:true,scaleLabel:{display:true,labelString:'Dato'}}],yAxes:[{display:true,scaleLabel:{display:true,labelString:'V&aelig;rdi'},ticks:{suggestedMin:-20,suggestedMax:250,}}],}}};$.each(config.data.datasets,function(i,dataset){dataset.borderColor=randomColor(1.0);dataset.borderColor=randomColor(1.0);dataset.pointBorderColor=randomColor(1.0);dataset.pointBackgroundColor=randomColor(1.0);dataset.pointBorderWidth=1;});window.onload=function(){var ctx=document.getElementById('canvas').getContext('2d');window.myLine=new Chart(ctx,config);};</script>";
const char eeprom_json[]           = "";
const char eeprom_template[]       = "<p class=\"log_line\"><span class=\"temperature_text\">%2.2f</span><span class=\"moisture_text\">%d%</span></p>\n";


void eeprom_testData(void);
void eeprom_logData(void);
void eeprom_initEeprom(void);
void eeprom_dumpLogData(void);

#endif	/* EEPROM_H */


