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

#define PRESCALER_DIVSION(timer_clk_src_freq , uart_baudrate) (timer_clk_src_freq / uart_baudrate)

#if (VCOM_FRAME_FORMAT == VCOM_FRAME_8N1)
    #define VCOM_FRAME_BITS 8
#elif //TODO

#endif

/**
 * @brief vcom handle structure definition
 *
 */
typedef struct vcom_handle_s
{
    uint8_t (*delay_timer_init)(uint32_t prescaler);
    uint8_t (*delay_timer_deinit)(void);
    // uint8_t (*delay_timer_irq_callback)(void);
    uint8_t (*tx_gpio_init)(void);
    uint8_t (*tx_gpio_deinit)(void);
    uint8_t (*tx_gpio_write)(uint8_t level);
#if (VCOM_TRANSMIT_MODE == VCOM_NONBLOCK)
    uint8_t (*delay_timer_start)(void);
    uint8_t (*delay_timer_stop)(void);
    uint8_t (*tx_done_callback)(void);  // callback in delay timer irq handler, used carefaully
#elif (VCOM_TRANSMIT_MODE == VCOM_BLOCK)
    uint8_t (*delay_timer_wait)(void);
#elif (VCOM_TRANSMIT_MODE == VCOM_FIFO)
    uint8_t (*delay_timer_start)(void);
    uint8_t (*delay_timer_stop)(void);
    uint8_t cache[VCOM_FIFO_CACHE_SIZE];
#else
    #error "VCOM_TRANSMIT_MODE error define"
#endif
    uint32_t baudrate;
    volatile uint8_t tx_done;
    uint8_t inited;
    // uint8_t endian;
    // uint32_t tick;
    // uint32_t transfer_ticks;
    char * tx_buf;
    uint32_t tx_buf_len;
    uint8_t encoded_bits;
    uint32_t encoded_bytes;
    volatile uint8_t frame_index;
    uint8_t frame[VCOM_FRAME_BITS];      // max 8bits + 1bits parity
} vcom_handle_t;

uint8_t vcom_init(vcom_handle_t * handle, uint32_t baudrate);

uint8_t vcom_frame_encode(vcom_handle_t *handle);

uint8_t vcom_transmit(vcom_handle_t *handle, char *buf, size_t len);

void vcom_transfer(vcom_handle_t *handle);

uint8_t vcom_deinit(vcom_handle_t * handle);

#endif
