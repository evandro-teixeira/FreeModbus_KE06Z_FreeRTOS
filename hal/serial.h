/*
 * serial.h
 *
 *  Created on: 04/09/2019
 *      Author: evandro
 */

#ifndef HAL_SERIAL_H_
#define HAL_SERIAL_H_


#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_port.h"


void serial_init(uint32_t baudrate);
void serial_enable_interrupt(void);
void serial_enable_rx_interrupt(void);
void serial_disable_rx_interrupt(void);
void serial_enable_tx_interrupt(void);
void serial_disable_tx_interrupt(void);
void serial_put_byte(char byte);
uint8_t serial_get_byte(void);

void serial_callback(void (*task)(void));

uint32_t serial_status_rx_flag(void);
uint32_t serial_status_tx_flag(void);

#endif /* HAL_SERIAL_H_ */
