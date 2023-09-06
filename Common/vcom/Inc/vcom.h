/**
 * @file    vcom.h
 * @brief   Simulate UART send use GPIO
 * @version 1.0.0
 * @author  Simon Law
 * @date    2023-09-04
 */


#ifndef __VCOM_H
#define __VCOM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "vcom_conf.h"

#if (VCOM_FRAME_FORMAT == VCOM_FRAME_8N1)
    #define VCOM_FRAME_BITS 9
#elif //TODO

#endif

/**
 * @brief vcom handle structure definition
 *
 */
typedef struct vcom_handle_s
{
    uint8_t (*delay_timer_init)(void);
    uint8_t (*delay_timer_deinit)(void);
    uint8_t (*delay_timer_start)(void);
    uint8_t (*delay_timer_stop)(void);
    // uint8_t (*delay_timer_irq_callback)(void);
    uint8_t (*tx_gpio_init)(void);
    uint8_t (*tx_gpio_deinit)(void);
    uint8_t (*tx_gpio_write)(uint8_t level);
    uint8_t (*tx_done_callback)(void);  // callback in delay timer irq handler, used carefaully
    uint8_t inited;
    // uint8_t endian;
    uint8_t tx_done;
    uint32_t tick;
    uint32_t transfer_ticks;
    char * tx_buf;
    uint32_t tx_buf_len;
    uint8_t encoded_bits;
    uint32_t encoded_bytes;
    uint8_t frame_index;
    uint8_t frame[VCOM_FRAME_BITS];      // max 8bits + 1bits parity
} vcom_handle_t;

uint8_t vcom_init(vcom_handle_t * handle);

uint8_t vcom_frame_encode(vcom_handle_t *handle);

uint8_t vcom_tx(vcom_handle_t *handle, char *buf, size_t len);

void vcom_delay_timer_irq_callback(vcom_handle_t *handle);

uint8_t vcom_deinit(vcom_handle_t * handle);

#endif
