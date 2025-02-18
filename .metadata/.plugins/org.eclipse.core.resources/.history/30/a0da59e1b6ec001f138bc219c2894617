/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Feb 16, 2025
 *      Author: furkanserper
 */

#include "stm32f407xx_i2c_driver.h"





void I2C_Init(I2C_Handle_t *pI2CHandle)
{

}


void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{



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
