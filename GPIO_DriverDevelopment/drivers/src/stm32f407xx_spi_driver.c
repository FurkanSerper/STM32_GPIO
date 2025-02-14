/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Feb 14, 2025
 *      Author: furkanserper
 */

#include "stm32f407xx_spi_driver.h"



/*
 * Peripheral Clock Setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}

	}else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DIS();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DIS();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DIS();
		}else if(pSPIx == SPI4)
		{
			SPI4_PCLK_DIS();
		}
	}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

}


void SPI_DeInit(SPI_RegDef_t *pSPIx)
{


}


/*
 *  Data send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{


}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{


}


/*
 * IRQ Confifuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{


}


void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{


}


void SPI_IRQHandling(SPI_Handle_t *pHandleSPI)
{


}
