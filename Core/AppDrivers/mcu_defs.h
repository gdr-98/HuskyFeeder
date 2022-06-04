#ifndef MCU_DEF
#define MCU_DEF
//	HAL definition
#define	STM32_HAL	1
//	MCU	includes
#if STM32_HAL
#include "stm32f4xx_hal.h"
#endif
#define __ALWAYSINLINE __attribute__((always_inline)) inline
//	MCU	abstractions
typedef	GPIO_TypeDef* 	pin_port;
typedef	uint16_t		pin_number;

//	Pin types and macros
#define	PIN_HIGH						GPIO_PIN_SET
#define	PIN_LOW							GPIO_PIN_RESET
#define	PIN_WRITE(port,number,state)	HAL_GPIO_WritePin(port,number,state)
#define	PIN_READ(port,number)			HAL_GPIO_ReadPin(port,number)

//Returns current tick counter value in ms
#define	GET_MS()						HAL_GetTick()
#define DELAY_MS(ms)					HAL_Delay(ms)


#endif
