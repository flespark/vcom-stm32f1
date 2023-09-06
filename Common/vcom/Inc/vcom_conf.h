#ifndef __VCOM_CONF_H
#define __VCOM_CONF_H

#include <stdio.h>
#include <stdint.h>
#include <string.h> 

#define VCOM_FRAME_8N1  0   // 8 bits data, none parity, 1 bit stop
#define VCOM_FRAME_8O1  1
#define VCOM_FRAME_8E1  2
#define VCOM_FRAME_8R1  3
#define VCOM_FRAME_8S1  4
//TODO: more uart frame support

#define VCOM_FRAME_FORMAT VCOM_FRAME_8N1
#define VCOM_BAUDRATE   9600
#define VCOM_DELAY_TIMER_IRQ_HZ 9600

#endif