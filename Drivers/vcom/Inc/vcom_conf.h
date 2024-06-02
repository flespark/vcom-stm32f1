#ifndef __VCOM_CONF_H
#define __VCOM_CONF_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief available transmit interface call mode
 *
 */
// thread nonblock mode, provide transmit done flag and callback, call under busy return non-zero
#define VCOM_NONBLOCK   0
// thread block mode, call under busy return non-zero
#define VCOM_BLOCK      1
// fifo cache mode, call too often to overflow return nono-zero
#define VCOM_FIFO       2

/**
 * @brief transmit interface call mode setting
 *
 */
#define VCOM_TRANSMIT_MODE VCOM_NONBLOCK

/**
 * @brief fifo cache size
 *
 */
#if (VCOM_TRANSMIT_MODE == VCOM_FIFO)
#define VCOM_FIFO_CACHE_SIZE 2048
#endif

/**
 * @brief delay timer clock source frequency for caculate prescaler under timer init interface
 *
 */
#define VCOM_TIMER_CLOCK_SOURCE_FREQ 4500000

/**
 * @brief available frame format, UNDER CONSTRUCTION
 *
 */
#define VCOM_FRAME_8N1  0   // 8 bits data, none parity, 1 bit stop
#define VCOM_FRAME_8O1  1
#define VCOM_FRAME_8E1  2
#define VCOM_FRAME_8R1  3
#define VCOM_FRAME_8S1  4

/**
 * @brief frame format setting
 *
 */
#define VCOM_FRAME_FORMAT VCOM_FRAME_8N1

#endif
