#include "vcom.h"
#include "vcom_conf.h"

uint8_t vcom_init(vcom_handle_t * handle)
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
    if (handle->delay_timer_start == NULL) {
        return 5;
    }
    if (handle->delay_timer_stop == NULL) {
        return 6;
    }

    if (handle->delay_timer_init() != 0) {
        return 7;
    }

    if (handle->tx_gpio_init() != 0) {
        return 8;
    }

    handle->tx_done = 1;    // alse indicate vcom if busy
    return 0;
}

uint8_t vcom_frame_encode(vcom_handle_t *handle)
{
#if (VCOM_FRAME_FORMAT == VCOM_FRAME_8N1)
    handle->encoded_bits = 0;
    for (uint8_t i = 0; i < VCOM_FRAME_BITS; i++) {
        handle->frame[i] = handle->tx_buf[handle->encoded_bytes] & (1 << handle->encoded_bits);
        handle->encoded_bits++;
    }
    handle->encoded_bytes++;
    handle->frame_index = 0;
#elif   //TODO: support other frame
#endif
    return 0;
}

uint8_t vcom_tx(vcom_handle_t *handle, char *buf, size_t len)
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
    memset(handle->frame, 1, VCOM_FRAME_BITS);  // start frame

    handle->tx_gpio_write(0);
    handle->delay_timer_start();
    return 0;
}

void vcom_delay_timer_irq_callback(vcom_handle_t *handle)
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
        handle->frame_index = 1;
        handle->tx_gpio_write(1);
        handle->frame_index++;
    } else {
        if (handle->encoded_bytes < handle->tx_buf_len) {
            handle->tx_gpio_write(0);
            vcom_frame_encode(handle);
        } else {
            handle->tx_gpio_write(1);
            handle->delay_timer_stop();
            handle->tx_done = 1;
            if (handle->tx_done_callback != NULL) {
                handle->tx_done_callback();
            }
        }
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

    return 0;
}

