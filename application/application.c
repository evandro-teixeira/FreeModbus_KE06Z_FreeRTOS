/*
 * application.c
 *
 *  Created on: 01/09/2019
 *      Author: evandro
 */

#include "application.h"

#define SIZE_TASK_APP		(configMINIMAL_STACK_SIZE + 10)
#define PRIORITIES_APP		(configMAX_PRIORITIES - 1)
#define SIZE_TASK_MODBUS	(configMINIMAL_STACK_SIZE + 10)
#define PRIORITIES_MODBUS	(configMAX_PRIORITIES - 1)

#define REG_INPUT_START 				1000
#define REG_INPUT_NREGS 				8
#define REG_HOLDING_START           	2000
#define REG_HOLDING_NREGS           	50
#define REG_COILS_START					0
#define REG_COILS_NREGS					50
#define REG_DISCRETE_COILS_START		100
#define REG_DISCRETE_NREGS				50

static UCHAR usRegDiscreteCoilsStart 	= REG_DISCRETE_COILS_START;
static UCHAR usRegCoilsStart 			= REG_COILS_START;
static USHORT usRegHoldingStart 		= REG_HOLDING_START;
static USHORT usRegInputStart 			= REG_INPUT_START;
static UCHAR usRegDiscreteCoilsBuf[REG_DISCRETE_NREGS];
static UCHAR usRegCoilsBuf[REG_COILS_NREGS];
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
static USHORT usRegInputBuf[REG_INPUT_NREGS];

/**
 *
 */
static void modbus_task(void *pvParameters);
static void app_task(void *pvParameters);

/**
 *
 */
static void modbus_task(void *pvParameters)
{
//	PRINTF("\n\rCreat Task Modbus");

	// Inicializa modbus
	eMBInit( MB_RTU, 1, 0, 115200, MB_PAR_NONE );
	eMBEnable();

	while(1)
	{
		eMBPoll();
		vTaskDelay(1);
	}
}

/**
 *
 */
static void app_task(void *pvParameters)
{
//	PRINTF("\n\rCreat Task APP");

	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0,};

    /* Init output LED GPIO. */
    GPIO_PinInit(kGPIO_PORTG, 5U, &led_config);

	while(1)
	{
		GPIO_PortToggle(kGPIO_PORTG, 1u << 5U);
		vTaskDelay(1000);
	}
}

/**
 *
 */
void modbus_task_init(void)
{
	 if (xTaskCreate(modbus_task,"Modbus",SIZE_TASK_MODBUS,NULL,PRIORITIES_MODBUS,NULL) != pdPASS)
	 {
		 /* Task creation failed! */
		 uint32_t i = 0;

		 while (1)
		 {
			 i++;
		 }
	 }
}

/**
 *
 */
void app_task_init(void)
{
	 if (xTaskCreate(app_task,"APP",SIZE_TASK_APP,NULL,PRIORITIES_APP,NULL) != pdPASS)
	 {
		 /* Task creation failed! */
		 uint32_t i = 0;

		 while (1)
		 {
			 i++;
		 }
	 }
}

/**
 *
 *
 */
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if(( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ))
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 *
 *
 */
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    //return MB_ENOREG;
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex;

	if((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		switch(eMode)
		{
		/* Pass current register values to the protocol stack. */
		case MB_REG_READ:
			while(usNRegs > 0)
			{
				*pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] >> 8);
				*pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
			break;

			/* Update current register values with new values from the
			 * protocol stack. */
		case MB_REG_WRITE:
			while(usNRegs > 0)
			{
				usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
				usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}

/**
 *
 *
 */
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
   // return MB_ENOREG;
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if((usAddress >= REG_COILS_START) && (usAddress + usNCoils <= REG_COILS_START + REG_COILS_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegCoilsStart);

        switch(eMode)
        {
            case MB_REG_READ:
            {
                while(usNCoils > 0)
                {
                    UCHAR ucResult = xMBUtilGetBits(usRegCoilsBuf, iRegIndex, 1);
                    xMBUtilSetBits(pucRegBuffer, iRegIndex - (usAddress - usRegCoilsStart), 1, ucResult);
                    iRegIndex++;
                    usNCoils--;
                }
                break;
            }
            case MB_REG_WRITE:
            {
                while ( usNCoils > 0 )
                {
                    UCHAR ucResult = xMBUtilGetBits(pucRegBuffer, iRegIndex - (usAddress - usRegCoilsStart), 1);
                    xMBUtilSetBits(usRegCoilsBuf, iRegIndex, 1, ucResult );
                    iRegIndex++;
                    usNCoils--;
                }
                break;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 *
 *
 */
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    //return MB_ENOREG;
	eMBErrorCode eStatus = MB_ENOERR;
	int  iRegIndex;

	if((usAddress >= REG_DISCRETE_COILS_START) && (usAddress + usNDiscrete <= REG_DISCRETE_COILS_START + REG_DISCRETE_NREGS))
	{
		iRegIndex = (int)(usAddress - usRegDiscreteCoilsStart);
		while(usNDiscrete > 0)
		{
			UCHAR ucResult = xMBUtilGetBits(usRegDiscreteCoilsBuf, iRegIndex, 1);
			xMBUtilSetBits(pucRegBuffer, iRegIndex - (usAddress - usRegDiscreteCoilsStart), 1, ucResult);
			iRegIndex++;
			usNDiscrete--;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
