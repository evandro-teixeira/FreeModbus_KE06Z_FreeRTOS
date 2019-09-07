/*
 * FreeModbus Libary: Atmel AT91SAM3S Demo Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: portserial.c,v 1.1 2010/06/06 13:07:20 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "../include/mb.h"
#include "port.h"
#include "../include/mbport.h"
#include "../hal/serial.h"

/* ----------------------- Defines ------------------------------------------*/


/* ----------------------- Static variables ---------------------------------*/
static BOOL bTXEnabled;
static BOOL bRXEnabled;

/* ----------------------- static functions ---------------------------------*/
static void prvvMBPortSerialISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{

    if( xRxEnable )
    {
    	bRXEnabled = TRUE;
    	serial_enable_rx_interrupt();
    }
    else
    {
    	bRXEnabled = FALSE;
    	serial_disable_rx_interrupt();
    }

    if( xTxEnable )
    {
    	bTXEnabled = TRUE;
    	serial_enable_tx_interrupt();
    }
    else
    {
    	bTXEnabled = FALSE;
    	serial_disable_tx_interrupt();
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bStatus = FALSE;

    serial_init(ulBaudRate);
    serial_callback(prvvMBPortSerialISR);
    serial_enable_interrupt();
    bStatus = TRUE;
    return bStatus;
}

void
vMBPortSerialClose( void )
{

}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	serial_put_byte(ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	*pucByte = serial_get_byte();
    return TRUE;
}

/*
 * IRQ Uart
 */
static void prvvMBPortSerialISR( void )
{
	if(bTXEnabled && serial_status_tx_flag())
	{
		 ( void )pxMBFrameCBTransmitterEmpty(  );
	}

	if(bRXEnabled && serial_status_rx_flag())
	{
		( void )pxMBFrameCBByteReceived(  );
	}
}



