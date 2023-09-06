#ifndef __VCOM_INTERFACE_H
#define __VCOM_INTERFACE_H

#include "vcom.h"

uint8_t vcom_interface_timer_init(void);
uint8_t vcom_interface_timer_deint(void);
uint8_t vcom_interface_timer_start(void);
uint8_t vcom_interface_timer_stop(void);
uint8_t vcom_interface_tx_gpio_init(void);
uint8_t vcom_interface_tx_gpio_deinit(void);
uint8_t vcom_interface_tx_gpio_wirte(uint8_t level);

#endif