#ifndef	PORT_SERVO
#define	PORT_SERVO
/*********************
 * FILE DESCR:
 *
 *	This file is a portability layer that abstracts the presence sensor functions for the Husky Feeder App.
 *	It basically consists in a set of definitions and macros
 *********************/
#define	PRESENCE_STM32F446RE	1
#define ABSTRACTIONS			0
#if PRESENCE_STM32F446RE
// In main.h should be defined PIR_PIN_Pin and PIR_PIN_Pin and PIR_PIN_GPIO_Port
#include "main.h"
//Returns current tick counter value in ms
#define	HFEED_PRESENCE_GET_MS()	HAL_GetTick()
#endif

#if ABSTRACTIONS
typedef struct{
	/* Number of times the objext has been detected*/
	uint16_t times_counter;
	/* The maximum number of times the object needs to be detected*/
	uint16_t max_value;
	/* The starting time of the object,setted as reset value */
	uint32_t starting_time;
	/*	The ending time */
	uint32_t ending_time;
}Presence_Manager;

static Presence_Manager* pvt_ptr;
#define HFEED_PM_GET(id) pvt_ptr;
inline bool reset_manager(PIR_Manager* manager,uint16_t max_value ,uint32_t ending_time){
	if (max_value==0||ending_time==0)
		return false;
	manager->times_counter=0;
	manager->max_value=max_value;
	manager->ending_time=ending_time;
	manager->starting_time=HFEED_PRESENCE_GET_MS();
}
#endif

static uint32_t counter=0;
/* Checks if actually the IC has detected an object*/
#define HFEED_IS_PRESENT_NOW()	HAL_GPIO_ReadPin(PIR_PIN_GPIO_Port, PIR_PIN_Pin)==GPIO_PIN_RESET
/* Checks if actually the IC has detected an object when using the IC in isr mode*/
#define HFEED_IS_PRESENT_ISR()	counter>0
#define HFEED_ISR_UPDATE_WM()	counter++;
/* After resetting you should check if the Dog is present with HFEED_IS_PRESENT_NOW */
#define HFEED_RESET_CTR()		ctr=0


#endif
