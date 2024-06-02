#include "vcom.h"
#include "vcom_conf.h"

uint8_t vcom_init(vcom_handle_t * handle, uint32_t baudrate)
{
    if (handle->inited) {
        return 1;
    }
    if (handle->delay_timer_init == NULL) {
        return 2;
    }
    if (handle->tx_gpio_init == NULL) {
        return 3;
    }
    if (handle->tx_gpio_write == NULL) {
        return 4;
    }
#if (VCOM_TRANSMIT_MODE == VCOM_NONBLOCK) || (VCOM_TRANSMIT_MODE == VCOM_FIFO)
    if (handle->delay_timer_start == NULL) {
        return 5;
    }
    if (handle->delay_timer_stop == NULL) {
        return 6;
    }
#elif (VCOM_TRANSMIT_MODE == VCOM_BLOCK)
    if (handle->delay_timer_wait == NULL) {
        return 5;
    }
#endif

    // check if baudrate GT timer clock source frequency or fruency bias GT 5% after division
    if (baudrate > VCOM_TIMER_CLOCK_SOURCE_FREQ ||
        (float)(VCOM_TIMER_CLOCK_SOURCE_FREQ % baudrate) / (float)VCOM_TIMER_CLOCK_SOURCE_FREQ > (float)5) {
        return 9;
    } else {
        handle->baudrate = baudrate;
    }

    if (handle->delay_timer_init(PRESCALER_DIVSION(VCOM_TIMER_CLOCK_SOURCE_FREQ, handle->baudrate)) != 0) {
        return 7;
    }

    if (handle->tx_gpio_init() != 0) {
        return 8;
    }

    handle->inited = 1;
    handle->tx_done = 1;    // also indicate vcom if busy
    return 0;
}

uint8_t vcom_frame_encode(vcom_handle_t *handle)
{
#if (VCOM_FRAME_FORMAT == VCOM_FRAME_8N1)
    handle->encoded_bits = 0;
    for (uint8_t i = 0; i < VCOM_FRAME_BITS; i++) {
        handle->frame[i] = (handle->tx_buf[handle->encoded_bytes] >> handle->encoded_bits) & 1;
        handle->encoded_bits++;
    }
    handle->encoded_bytes++;
#elif   //TODO: support other frame
#endif
    return 0;
}

uint8_t vcom_transmit(vcom_handle_t *handle, char *buf, size_t len)
{
    if (handle->inited != 1) {
        return 1;
    }

    if (buf == NULL) {
        return 2;
    }

    if (len <= 0) {
        return 3;
    }

    if (handle->tx_done != 1) {
        return 4;
    } else {
        handle->tx_done = 0;
    }

    handle->tx_buf = buf;
    handle->tx_buf_len = len;
    handle->encoded_bits = 0;
    handle->encoded_bytes = 0;
    handle->frame_index = 0;
    // memset(handle->frame, 1, VCOM_FRAME_BITS);  // start frame
    vcom_frame_encode(handle);
    handle->tx_gpio_write(0);
#if (VCOM_TRANSMIT_MODE == VCOM_NONBLOCK)
    handle->delay_timer_start();
#elif (VCOM_TRANSMIT_MODE == VCOM_BLOCK)
    while (handle->tx_done != 1) {
        handle->delay_timer_wait();
        vcom_transfer(handle);
    }
#else
    #error "VCOM_FIFO support under constuction"
#endif
    return 0;
}

void vcom_transfer(vcom_handle_t *handle)
{
    // if (handle->inited != 1) {
    //     return;
    // }
    if (handle->tx_done == 1) {
        return;
    }

    if (handle->frame_index < VCOM_FRAME_BITS) {
        handle->tx_gpio_write(handle->frame[handle->frame_index]);
        handle->frame_index++;
    } else if (handle->frame_index == VCOM_FRAME_BITS) {
        //TODO: support other frame format
        handle->tx_gpio_write(1);
        handle->frame_index++;
    } else {
        if (handle->encoded_bytes < handle->tx_buf_len) {
            handle->tx_gpio_write(0);
            vcom_frame_encode(handle);
        } else {
            handle->tx_gpio_write(1);
#if (VCOM_TRANSMIT_MODE == VCOM_NONBLOCK)
            handle->delay_timer_stop();
            if (handle->tx_done_callback != NULL) {
                handle->tx_done_callback();
            }
#endif
            handle->tx_done = 1;
        }
        handle->frame_index = 0;
    }
}

uint8_t vcom_deinit(vcom_handle_t * handle)
{
    if (handle->inited != 1) {
        return 1;
    }

    if (handle->delay_timer_deinit != NULL) {
        handle->delay_timer_deinit();
    }

    if (handle->tx_gpio_deinit != NULL) {
        handle->tx_gpio_deinit();
    }

    handle->inited = 0;
    return 0;
}

