/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Feb 14, 2025
 *      Author: furkanserper
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_
#include "stm32f407xx.h"



/*
 * Configuration structure for SPIx peripheral
 */
typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;				/* Possible values from @SPI //TODO				*/
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;					/* Possible values from @SPI //TODO				*/
	uint8_t SPI_CPOL;					/* Possible values from @SPI //TODO 			*/
	uint8_t SPI_CPHA;					/* Possible values from @SPI //TODO				*/
	uint8_t SPI_SSM ;
}SPI_Config_t;

/*
 * Handle Structure for SPIx peripheral
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;  /* This hold the base address of the SPIx  peripheral*/
	SPI_Config_t SPIConfig;
}SPI_Handle_t;



/****************************************************************************************************
 * 								APIs supported by this driver
 * 			For more information abou the APIs check the function definitions
 ****************************************************************************************************/

/*
 * Peripheral Clock Setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);


/*
 *  Data send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);


/*
 * IRQ Confifuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandleSPI);



#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
