/*
 * stm32f407xx.h
 *
 *  Created on: Feb 7, 2025
 *      Author: furkanserper
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_


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





#endif /* INC_STM32F407XX_H_ */
