/*
 * spi_tx_testing.c
 *
 *  Created on: Feb 14, 2025
 *      Author: furkanserper
 */

/*
* PB15 --> SP2_MOSI
* PB14	-->	SP2_MISO
* PB13	-->	SP2_SCLK
* PB12 -->	SP2_NSS
* ALT function mode: 5
*/
#include "stm32f407xx.h"
#include "string.h"

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = 	GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO   This par can be disable as we are only sending
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins); // Clock Peripheral is already inside the GPIO_Init()

}

void 	SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;

	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;  //generates sclk of 8MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // Software slave management ena le for NSS pin

	SPI_Init(&SPI2handle); // Clock Peripheral is already inside the SPI_Init()


}

int main(void)
{

	char user_data[] = "Hello World";

	//This function is used to initialize the GPIO pins to behave as SPI2 Pins
	SPI2_GPIOInits();

	// This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	//This makes NSS signal internally high and avoids MODF Error
	SPI_SSIConfig(SPI2, ENABLE);

	//Enable the SPI2 peripherals
	SPI_PeriClockControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

	SPI_PeriClockControl(SPI2, DISABLE);


	return 0;
}
