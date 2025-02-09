/*
 * stm32f407xx.h
 *
 *  Created on: Feb 7, 2025
 *      Author: furkanserper
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_


#include <stdio.h>
#include <stdint.h>

#define __vo volatile

/* Check the value from Table 5. Flash module organization in Reference Manual */
#define FLASB_BASEADDR			0x08000000U		/* Flash Base Address */
#define SRAM1_BASEADDR			0x20000000U     /* SRAM1 Base Address */
#define SRAM2_BASEADDR			0x2001C000U     /* SRAM2 Base Address */
#define ROM_BASEADDR			0x1FFF0000U     /* System Memory - ROM Address*/
#define SRAM 					SRAM1_BASEADDR  /* Same with SRAM1 Base Address */


#define PERIPH_BASE				0x40000000U
#define APB1PERIPH_BASE			PERIPH_BASE
#define APB2PERIPH_BASE			0x40010000U
#define AHB1PERIPH_BASE         0x40020000U
#define AHB2PERIPH_BASE         0x50000000U


/* Base addresses of peripherals whic are hangion on AHb1 bus
 * TODO : complete for all other peripherals
 */


#define GPIOA_BASEADDR			(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR			(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR			(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR			(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR			(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR			(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR			(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR			(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR			(AHB1PERIPH_BASE + 0x2000)
#define RCC_BASEADDR			(AHB1PERIPH_BASE + 0x3800)

/*  Base addresses of peripherals which are hanging on APB1 bus    */

#define I2C1_BASEADDR 			(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR 			(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR 			(APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR			(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR			(APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR			(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR			(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR			(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR			(APB1PERIPH_BASE + 0x5000)

/*  Base addresses of peripherals which are hanging on APB2 bus    */
#define SPI1_BASEADDR			(APB2PERIPH_BASE + 0x3000)
#define USART1_BASEADDR			(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR			(APB2PERIPH_BASE + 0x1400)
#define EXTI_BASEADDR			(APB2PERIPH_BASE + 0x3C00)
#define SYSCFG_BASEADDR			(APB2PERIPH_BASE + 0x3800)



/* Peripheral Register Definition Structures     */

typedef struct
{
	__vo uint32_t MODER;			/*  GPIO port mode register    								*/
	__vo uint32_t OTYPER;		/*	GPIO port output type register							*/
	__vo uint32_t OSPEEDR;		/*	GPIO port output speed register							*/
	__vo uint32_t PUPDR;			/*	GPIO port pull-up/pull-down register					*/
	__vo uint32_t IDR;			/*	GPIO port input data register							*/
	__vo uint32_t ODR;			/*	GPIO port output data register							*/
	__vo uint32_t BSRR;			/*	GPIO port bit set/reset register						*/
	__vo uint32_t LCKR;			/*	GPIO port configuration lock register					*/
	__vo uint32_t AFR[2];		/*	GPIO alternate function low register - high register	*/
}GPIO_RegDef_t;


typedef struct
{
	__vo uint32_t  CR;
	__vo uint32_t  PLLCFGR;
	__vo uint32_t  CFGR;
	__vo uint32_t  CIR;
	__vo uint32_t  AHB1RSTR;
	__vo uint32_t  AHB2RSTR;
	__vo uint32_t  AHB3RSTR;
	uint32_t  RESERVED0;
	__vo uint32_t  APB1RSTR;
	__vo uint32_t  APB2RSTR;
	uint32_t  RESERVED1[2];
	__vo uint32_t  AHB1ENR;
	__vo uint32_t  AHB2ENR;
	__vo uint32_t  AHB3ENR;
	__vo uint32_t  RESERVED2;
	__vo uint32_t  APB1ENR;
	__vo uint32_t  APB2ENR;
	uint32_t  RESERVED3[2];
	__vo uint32_t  AHB1LPENR;
	__vo uint32_t  AHB2LPENR;
	__vo uint32_t  AHB3LPENR;
	uint32_t  RESERVED4;
	__vo uint32_t  APB1LPENR;
	__vo uint32_t  APB2LPENR;
	uint32_t  RESERVED5[2];
	__vo uint32_t  BDCR;
	__vo uint32_t  CSR;
	uint32_t  RESERVED6[2];
	__vo uint32_t  SSCGR;
	__vo uint32_t  PLLI2SCFGR;
	/* TODO : CKGATENR ?    DCKCFGR2 ? */

}RCC_RegDef_t;

/*  Peripheral Definition (Peripheral base addresses typecasted to xxx_RegDef_t	 */

#define GPIOA           ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB           ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC           ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD           ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE           ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF           ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG           ((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH           ((GPIO_RegDef_t *)GPIOH_BASEADDR)
#define GPIOI           ((GPIO_RegDef_t *)GPIOI_BASEADDR)

#define RCC				((RCC_RegDef_t *)RCC_BASEADDR)


/*
 * Clock Enable Macros for GPIOx Peripherals
 */
#define GPIOA_PCLK_EN()		(RCC->AHB1ENR |= (1<< 0))
#define GPIOB_PCLK_EN()		(RCC->AHB1ENR |= (1<< 1))
#define GPIOC_PCLK_EN()		(RCC->AHB1ENR |= (1<< 2))
#define GPIOD_PCLK_EN()		(RCC->AHB1ENR |= (1<< 3))
#define GPIOE_PCLK_EN()		(RCC->AHB1ENR |= (1<< 4))
#define GPIOF_PCLK_EN()		(RCC->AHB1ENR |= (1<< 5))
#define GPIOG_PCLK_EN()		(RCC->AHB1ENR |= (1<< 6))
#define GPIOH_PCLK_EN()		(RCC->AHB1ENR |= (1<< 7))
#define GPIOI_PCLK_EN()		(RCC->AHB1ENR |= (1<< 8))


/*
 * Clock Enable Macros for I2Cx Peripherals
 */
#define I2C1_PCLK_EN()		(RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()		(RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()		(RCC->APB1ENR |= (1<<23))


/*
 * Clock Enable Macros for SPIx Peripherals
 */

#define SPI1_PCLK_EN()		(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()		(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()		(RCC->APB1ENR |= (1<<15))
#define SPI4_PCLK_EN()		(RCC->APB2ENR |= (1<<13))
#define SPI5_PCLK_EN()		(RCC->APB2ENR |= (1<<20))
#define SPI6_PCLK_EN()		(RCC->APB2ENR |= (1<<21))



/*
 * Clock Enable Macros for USARTx Peripherals
 */
#define USART1_PCLK_EN()		(RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()		(RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN()		(RCC->APB1ENR |= (1<<18))
#define UART4_PCLK_EN()			(RCC->APB1ENR |= (1<<19))
#define UART5_PCLK_EN()			(RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN()		(RCC->APB2ENR |= (1<<5))
#define UART7_PCLK_EN()			(RCC->APB1ENR |= (1<<30))
#define UART8_PCLK_EN()			(RCC->APB1ENR |= (1<<31))



/*
 * Clock Enable Macros for SYSCFG Peripherals
 */
#define SYSCFG_PCLK_EN()        (RCC->APB2ENR |= (1<<14))




/*
 * DISABLE
 */
/*
 *
 *
 * Clock Enable Macros for GPIOx Peripherals
 */
#define GPIOA_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 0))
#define GPIOB_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 1))
#define GPIOC_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 2))
#define GPIOD_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 3))
#define GPIOE_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 4))
#define GPIOF_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 5))
#define GPIOG_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 6))
#define GPIOH_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 7))
#define GPIOI_PCLK_DIS()		(RCC->AHB1ENR &= ~(1<< 8))


/*
 * Clock Enable Macros for I2Cx Peripherals
 */
#define I2C1_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<23))


/*
 * Clock Enable Macros for SPIx Peripherals
 */

#define SPI1_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<13))
#define SPI5_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<20))
#define SPI6_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<21))



/*
 * Clock Enable Macros for USARTx Peripherals
 */
#define USART1_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<17))
#define USART3_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<18))
#define UART4_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<19))
#define UART5_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<20))
#define USART6_PCLK_DIS()		(RCC->APB2ENR &= ~(1<<5))
#define UART7_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<30))
#define UART8_PCLK_DIS()		(RCC->APB1ENR &= ~(1<<31))



/*
 * Clock Enable Macros for SYSCFG Peripherals
 */
#define SYSCFG_PCLK_DIS()        (RCC->APB2ENR &= ~(1<<14))


/*
 * Macros to reset GPIOx Peropherals
 */

#define GPIOA_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<0)); (RCC->AHB1RSTR &= ~(1<<0)); }while(0)
#define GPIOB_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<1)); (RCC->AHB1RSTR &= ~(1<<1)); }while(0)
#define GPIOC_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<2)); (RCC->AHB1RSTR &= ~(1<<2)); }while(0)
#define GPIOD_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<3)); (RCC->AHB1RSTR &= ~(1<<3)); }while(0)
#define GPIOE_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<4)); (RCC->AHB1RSTR &= ~(1<<4)); }while(0)
#define GPIOF_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<5)); (RCC->AHB1RSTR &= ~(1<<5)); }while(0)
#define GPIOG_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<6)); (RCC->AHB1RSTR &= ~(1<<6)); }while(0)
#define GPIOH_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<7)); (RCC->AHB1RSTR &= ~(1<<7)); }while(0)
#define GPIOI_REG_RESET()			do{(RCC->AHB1RSTR |= (1<<8)); (RCC->AHB1RSTR &= ~(1<<8)); }while(0)

/*
 * Generic Macros
 */

#define ENABLE 					1
#define DISABLE 				0
#define SET 					ENABLE
#define RESET					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET

#endif /* INC_STM32F407XX_H_ */
