/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Feb 14, 2025
 *      Author: furkanserper
 */

#include "stm32f407xx_spi_driver.h"
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle);

// -specs=rdimon.specs -lc -lrdimon
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

	//Peripheral Clock Enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register
	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI mode should be clread (set 0)
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set (set 1)
		tempreg |= (1 << SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be clread (set 0)
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

		// RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);

	}

	if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
	{
		tempreg |= (1 << SPI_CR1_SSM);
	}else if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_DIS)
	{
		tempreg &= ~(1 << SPI_CR1_SSM);
	}

	// 3. Configure the SPI Serial Clock Speed (Baud Rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// 5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;


	// 6. configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = tempreg;

}


void SPI_DeInit(SPI_RegDef_t *pSPIx)
{


}


uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{

	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;

	}
	return FLAG_RESET;
}



/*
 *  Data send and Receive   (Blocking API)
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		while( SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. Check the DFF bit in CR1
		if((pSPIx->CR1 & (1<<SPI_CR1_DFF)))
		{
			// 16 BIT DFF
			//1. load the data in to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			// 8 BIT DFF
			//1. load the data in to the DR
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}


	}

}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until RXNE is set
		while( SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. Check the DFF bit in CR1
		if((pSPIx->CR1 & (1<<SPI_CR1_DFF)))
		{
			// 16 BIT DFF
			//1. load the data from DR to Rx buffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t*)pRxBuffer++;
		}else
		{
			// 8 BIT DFF
			//1. load the data in to the DR
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}

	}

}

// Non-blocking API
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1 sace the Tx buffer address and Len infrmation in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mar the SPI state as busy in transmission so that no other code can take
		// over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3 Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1<< SPI_CR2_TXEIE);
	}
	//4. Data transmission will be handled by the ISR code (will implemet later)

	return state;
}


// Non-blocking API
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1 sace the Tx buffer address and Len infrmation in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2. Mar the SPI state as busy in transmission so that no other code can take
		// over same SPI peripheral until transmission is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3 Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1<< SPI_CR2_RXNEIE);
	}
	//4. Data transmission will be handled by the ISR code (will implemet later)

	return state;

}



/*
 * IRQ Confifuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//Program ISER0 Register
			*NVIC_ISER0 |= (1 << IRQNumber);

		}else if((IRQNumber > 31) & (IRQNumber < 64)) //32 to 63
		{
			//Program ISER1 Register
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));

		}else if((IRQNumber >= 64) & (IRQNumber < 96)) //64 to 95
		{
			//Program ISER2 Register
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
		}
	}else if(EnorDi == DISABLE)
	{
		if(IRQNumber <= 31)
		{
			//Program ICER0 Register
			*NVIC_ICER0 |= (1 << IRQNumber);

		}else if((IRQNumber > 31) & (IRQNumber < 64))
		{
			//Program ICER1 Register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}else if((IRQNumber >= 64) & (IRQNumber < 96))
		{
			//Program ICER2 Register
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}


}


void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount  = (iprx_section * 8) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}





void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;
	//First lets check for TXE
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if((temp1 == 1) && (temp2 == 1))
	{
		//handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}

	//check for RXNE
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if((temp1 == 1) && (temp2 == 1))
	{
		//handle RXNE
		spi_rxne_interrupt_handle(pSPIHandle);
	}

	//check for OVR Flag
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if((temp1 == 1) && (temp2 == 1))
	{
		//handle OVR
		spi_ovr_interrupt_handle(pSPIHandle);
	}
}


/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);

	}else if(EnOrDi == DISABLE)
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);

	}else if(EnOrDi == DISABLE)
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}


void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);

	}else if(EnOrDi == DISABLE)
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}



/*
 * Helper Functions
 */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//2. Check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF)))
	{
		// 16 BIT DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		// 8 BIT DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(pSPIHandle->TxLen == 0)
	{
		//TxLen is zero, so close the spi transmission and infirm the application that TX is over
		// This prevent unterruts from setting up op TXE
		SPI_CloseTransmission(pSPIHandle);

		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);

	}

}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//2. Check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF)))
	{
		// 16 BIT DFF
		//1. load the data in to the DR
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		(uint16_t*)pSPIHandle->pRxBuffer--;
	}else
	{
		// 8 BIT DFF
		//1. load the data in to the DR
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer--;
	}

	if(pSPIHandle->RxLen == 0)
	{
		//RxLen is zero, so close the spi transmission and infirm the application that RX is over
		// This prevent unterruts from setting up op TXE
		SPI_CloseReception(pSPIHandle);

		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);

	}
}

static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle)
{

	uint8_t temp1,temp2;
	//1 clear the OVR flag
	if(pSPIHandle->TxState!=SPI_BUSY_IN_TX)
	{
		temp1 = pSPIHandle->pSPIx->DR;
		temp2 = pSPIHandle->pSPIx->SR;
	}

	//2. inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}


void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{

}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;


}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}



__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEvent)
{
	//This is a weak implementation. The application may override this function
}
