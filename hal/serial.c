/*
 * serial.c
 *
 *  Created on: 04/09/2019
 *      Author: evandro
 */
#include "serial.h"

#define SERIAL_UART_IRQ UART1_IRQHandler

/**
 *
 */
void (*serial_task_irq)(void);


/**
 *
 */
void serial_init(uint32_t baudrate)
{
	 uart_config_t config;

	 /* pin 63,64 is configured as UART1_TX, UART1_RX */
	 PORT_SetPinSelect(kPORT_UART1, kPORT_UART1_RXPTC7_TXPTC6);

    /*
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = baudrate;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_BusClk));

    /* Enable RX interrupt. */
    serial_enable_rx_interrupt();
    //EnableIRQ(DEMO_UART_IRQn);
}

/**
 *
 */
void serial_enable_interrupt(void)
{
	EnableIRQ(UART1_IRQn);
}

/**
 *
 */
void serial_enable_rx_interrupt(void)
{
	UART_EnableInterrupts(UART1,kUART_RxDataRegFullInterruptEnable);
}

/**
 *
 */
void serial_disable_rx_interrupt(void)
{
	UART_DisableInterrupts(UART1,kUART_RxDataRegFullInterruptEnable);
}

/**
 *
 */
void serial_enable_tx_interrupt(void)
{
	UART_EnableInterrupts(UART1,kUART_TxDataRegEmptyInterruptEnable);
}

/**
 *
 */
void serial_disable_tx_interrupt(void)
{
	UART_DisableInterrupts(UART1,kUART_TxDataRegEmptyInterruptEnable);
}

/**
 *
 */
void serial_put_byte(char byte)
{
	while(!(UART_GetStatusFlags(UART1) & kUART_TxDataRegEmptyFlag));
	UART_WriteByte(UART1,(uint8_t)byte);
	serial_enable_tx_interrupt();
}

/**
 *
 */
uint8_t serial_get_byte(void)
{
	uint8_t byte = 0;
	while(!(UART_GetStatusFlags(UART1) & kUART_TxDataRegEmptyFlag));
	byte = UART_ReadByte(UART1);
	serial_enable_rx_interrupt();
	return byte;
}

/**
 *
 */
void serial_callback(void (*task)(void))
{
	if(task != NULL)
	{
		serial_task_irq = task;
	}
}

/**
 *
 */
uint32_t serial_status_rx_flag(void)
{
	return (uint32_t)(UART_GetStatusFlags(UART1) & kUART_RxDataRegFullFlag);
}

/**
 *
 */
uint32_t serial_status_tx_flag(void)
{
	return (uint32_t)(UART_GetStatusFlags(UART1) & kUART_TransmissionCompleteFlag);
}

/**
 *
 */
void SERIAL_UART_IRQ(void)
{
	if(serial_task_irq != NULL)
	{
		serial_task_irq();
	}
}

