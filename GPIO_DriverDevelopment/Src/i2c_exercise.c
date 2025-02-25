/*
 * i2c_exercise.c
 *
 *  Created on: Feb 18, 2025
 *      Author: furkanserper
 */

/*
 * EXERCISE     I2C Master (STM32 Discovery) and I2C Slave (Arduino Board) Communication
 *
 * when button on the STM32 board (master) is pressed, master should send data to the Arduino
 * board (slave). The data received by the Arduino board will be displayed on the serial monitor
 * terminal of the Arduino IDE
 *
 * 1- Use I2C SCL = 100kHz (Standart Mode)
 * 2- USer external pull up resistor (3.3kohm) for SDA and SCL line
 *
 * Note: if you don't have external pull up resistors, you can also try with activating STM32
 * I2C pin's internal pull up resistors
 *
 * SCL --> PB6
 * SDA --> PB9
 *
 */

#include "stm32f407xx.h"
#include "string.h"


I2C_Handle_t I2C1Handle;
#define MY_DEVICE_ADDRESS 0x61
#define SLAVE_ADDRESS 	  0x68

//some data
uint8_t some_data[] = "We are testing I2C Master Tx\n";


void delay(void)
{
	for(uint32_t i = 0; i<500000/2; i++)
	{

	}
}

/*
 * SCL --> PB6
 * SDA --> PB9
 */
void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPins);


}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;

	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_DEVICE_ADDRESS;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);

}

void 	GPIO_ButtonInit()
{
	GPIO_Handle_t GPIOBtn, GpioLed;

	//this is button GPIO configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOBtn);

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);

}

int main(void)
{
	GPIO_ButtonInit();

	// I2C Pin Initilization
	I2C1_GPIOInits();

	// I2C initilization
	I2C1_Inits();

	//Enable the I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//wait for the button press
	while(1)
	{
		while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//send some data to Slave
		I2C_MasterSendData(&I2C1Handle, some_data, strlen(some_data), SLAVE_ADDRESS);

	}


}
