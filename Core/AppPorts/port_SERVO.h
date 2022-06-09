#ifndef	PORT_SERVO
#define	PORT_SERVO
/*********************
 * FILE DESCR:
 *
 *	This file is a portability layer that abstracts the servo motor functions for the Husky Feeder App.
 *	It basically consists in a set of definitions and macros such as the 90 degrees Ton equivalent or the
 *	pwm control functions.
 *********************/
#define	SERVO_STM32F446RE	1

#if SERVO_STM32F446RE
#include "main.h"

/*******************
 * NOTES:
 *	20/05/2022
 *	We're doing stuff in a very easy way in order to reduce work for developing the app's basic functionalities.
 *	The Angle-PWM conversion is in this version empirically done and there is no ticks angle conversion function.
 *	The Freq is set on 50.000 Hz( currently on htim11)and the ARR reg is 999, so that 50.000/(999+1)=50 Hz (20 ms of Period)
 *	The timer currently ticks on every 0.02 ms(1/50.000) so conversions are:
 *	120 degrees =	2(Ton)/0.02=100 ticks 	( CCR1 value)
 *	90  degrees	=	1.5(Ton)/0.02=75 ticks 	( CCR1 value)
 *	0  	degrees	=	1(Ton)/0.02=50 ticks 	( CCR1 value)
 *	So for example ideally we should set for a 90 deg rotation CCR value to 75-1.
 *	We've empirically seen that  180 and 90 it's better, in our cfg, to add a "" tollerance"" value to the CCR1 ticks calculated up her
 *	(15 was chosen),in order to make the motor turn perfectly.
 *	HOWTO:
 *		After the config of the timer and the HAL_TIM_PWM_Start call, from main of in a cfg function HFEED_SERVO_MCU_SET_PWM_CFG and HFEED_SERVO_MCU_SET_TIMCH.
 *		After this the application will be free to call the exported macros.
 *******************/

//	Some known angles ids ...
#define	HFEED_SERVO_ZERO_DG			50
#define HFEED_SERVO_NINETY_DG		90	//	75+15
#define	HFEED_SERVO_ONEHUNTWELVE_DG	115	//	100+15
/*
const	dg_to_pwm[]={
		50,
		90,	//	75+15
		115	//	100+15
};*/

//	macro used to convert deg in Ton
//	In STM32 Ton == CCR counter values
#define HFEED_SERVO_DEG2TON(angle)	angle

extern	TIM_HandleTypeDef htim11;
#define	stm32_tim_channel TIM_CHANNEL_1

//	Macros used from the write functions, every MCU port must implement these two
//	Checks if the angle is valid according to MCU definitions
#define HFEED_SERVO_VALID_ANGLE(angle)	(angle>=HFEED_SERVO_ZERO_DG && angle<=HFEED_SERVO_ONEHUNTWELVE_DG)
//	Sets a specific pwm
#define	HFEED_SERVO_MCU_SET_Ton(Ton)	 __HAL_TIM_SET_COMPARE(&htim11,stm32_tim_channel,Ton)

#endif

//Exported macros
#define	HFEED_SERVO_SET_ORIG()		HFEED_SERVO_MCU_SET_Ton(HFEED_SERVO_ZERO_DG)
#define	HFEED_SERVO_TURN_90()		HFEED_SERVO_MCU_SET_Ton(HFEED_SERVO_NINETY_DG)
#define	HFEED_SERVO_TURN_120()		HFEED_SERVO_MCU_SET_Ton(HFEED_SERVO_ONEHUNTWELVE_DG)
#define	HFEED_SERVO_TURN_ANGLE(angle){	\
	if(HFEED_SERVO_VALID_ANGLE(angle))		\
		HFEED_SERVO_MCU_SET_Ton(HFEED_SERVO_DEG2TON(angle));	\
}

#endif
/*
 * DEPRECATED
//	On stm32 this macro takes the tim cfg in input, the timer should be already configured and started with HAL PWM STARTs
#define	HFEED_SERVO_MCU_SET_PWM_CFG(cfg){	\
		if(cfg==0)							\
			Error_Handler();				\
		my_timer=cfg;						\
}

//	Function specific for stm32 implementation
#define	HFEED_SERVO_MCU_SET_TIMCH(timCH){	\
	if(timCH!=TIM_CHANNEL_1 &&timCH!=TIM_CHANNEL_2 && timCH!=TIM_CHANNEL_3 && timCH!=TIM_CHANNEL_4)	\
		Error_Handler();																				\
	stm32_tim_channel=timCH;																		\
}*/
