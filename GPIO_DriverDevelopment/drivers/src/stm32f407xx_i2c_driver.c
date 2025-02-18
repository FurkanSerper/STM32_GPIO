/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Feb 16, 2025
 *      Author: furkanserper
 */

#include "stm32f407xx_i2c_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,128,256,512};
uint16_t APB1_PreScaler[8] = {2,4,8,16};

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);   //Clear the 0th bit  (SlaveAddr is Slave address + R/not_w   0

	pI2Cx->DR = SlaveAddr;
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= (1);   //Clear the 0th bit  (SlaveAddr is Slave address + R/not_w  1

	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;

	(void)dummyRead;

}

static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);

}



/*
 * Peripheral Clock Setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}

	}else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DIS();
		}else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DIS();
		}else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DIS();
		}
	}
}


void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;
	uint8_t trise;

	//Enable the Clock for the I2C peripheral
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	// ACK control
	tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << 10);
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//configure the FREQ field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	// Program the device own address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress <<1;

	//Reserved bit (Reference Manual says 14th bit should be 1)
	tempreg |= (1<<14);

	pI2CHandle->pI2Cx->OAR1 = tempreg;


	//CCR calculation
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// Mode is Standard mode
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);

		tempreg |= (ccr_value & 0xFFF);


	}else
	{
		// Mode is fast mode
		tempreg |= (1<<15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);

		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);

		}else if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_16_9)
		{
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);

		}
		tempreg |= (ccr_value & 0xFFF);

	}

	pI2CHandle->pI2Cx->CCR = tempreg;

	// TRISE configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// Mode is standart Mode
		trise = (RCC_GetPCLK1Value() / 1000000U) + 1;
	}else
	{
		// Mode is fast Mode
		trise = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}

	pI2CHandle->pI2Cx->TRISE = (trise & 0x3F);

}


void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{



}



/*
 * Data send and receive
 */

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. GEnerate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	// Note: until SB is cleared SCL will be stretched (pulled to LOW)
	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)));

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. confirm that address phase is completed by checking the ADDR flag in the SR1
	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR)));


	//5. Clear the ADDR flag according to its software sequence
	// Note: until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. Sen the data until Len becomes 0

	while(Len > 0)
	{
		while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE))); //wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;

	}

	//7. When Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	// Note TXE=1 BTF=1 means that both SR and DR are empty and next transmission should begin
	// When BTF=1 SCL will be stretched (pulled to LOW)

	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE))); //wait till TXE is set

	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF))); //wait till BTF is set

	//8. generate STOP condigiton and master need not to wait for the completion of stop condition
	// Note: generating STOP, automatically clears the BTF
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);


	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	//Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)));

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);


	//4. wait until address phase is completed by checking the ADDR flag in the SR1
	while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR)));


	//procedure to read only 1 byte from slae
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//wait until RXNE becomes 1
		while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE))); //wait till TXE is set

		//generate STOP condigiton
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);


		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;


	}


	//Procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//Clear the ADDR Flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//read the data until Len becomes zero
		for(uint32_t i = Len; i>0; i--)
		{
			//Wait until RXNE becomes 1
			while(! (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE))); //wait till TXE is set


			if(i == 2 ) // if last 2 bytes are remaining
			{

				//Disable the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				//generate Stop condition
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);


			}

			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;


			//increment the buffer address
			pRxBuffer++;
		}
	}

	// Re-enable ACKing
	if((pI2CHandle->I2C_Config.I2C_ACKControl) == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}

}





uint32_t RCC_GetPCLK1Value()
{
	uint32_t pclk1,SystemClk;

	uint8_t clksrc, temp,ahbp, temp2, apb1;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0)
	{
		SystemClk = 16000000;  // HSI clock

	}else if(clksrc == 1)
	{
		SystemClk = 8000000;  // HSI clock
	}else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}


	// AHB
	temp = ((RCC->CFGR >> 4) & 0xF);

	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_PreScaler[temp-8];
	}


	// ABP1
	temp2 = ((RCC->CFGR >> 10) & 0x7);

	if(temp2 < 4)
	{
		apb1 = 1;
	}
	else
	{
		apb1 = APB1_PreScaler[temp-4];

	}


	pclk1 = ((SystemClk / ahbp) / apb1);
	return pclk1;
}

uint32_t RCC_GetPLLOutputClock(void)
{
	return 0;
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);

	}else if(EnOrDi == DISABLE)
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}


}



void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//Enable the ACK
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);

	}else if(EnorDi == I2C_ACK_DISABLE)
	{
		//Disable the ACK
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}




/*
 * IRQ Confifuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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


void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount  = (iprx_section * 8) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;

	}
	return FLAG_RESET;
}
