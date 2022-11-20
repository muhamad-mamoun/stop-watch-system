/*
===========================================================================================================
Author       : Mamoun
Project      : Stop Watch System
Date Created : Sep 15, 2022
Description  : 
===========================================================================================================
*/


/*===========================================================================================================
                                               < Includes >
===========================================================================================================*/

#include "../MCAL/gpio.h"
#include "../MCAL/timer1.h"
#include "../MCAL/external_interrupts.h"
#include <util/delay.h>
#include <avr/io.h>

/*===========================================================================================================
                                < Definitions and Static Configurations >
===========================================================================================================*/

#define SELECTION_PORT_ID          (PORTA_ID)
#define FIRST_SELECTION_PIN_ID     (PIN0_ID)
#define NUMBER_OF_SELECTIONS       (6u)

#define DECODER_PORT_ID            (PORTC_ID)
#define FIRST_DECODER_PIN_ID       (PIN0_ID)

#define DISPLAY_DELAY              (2)

/*===========================================================================================================
                                           < Global Variables >
===========================================================================================================*/

typedef struct
{
	char seconds;
	char minutes;
	char hours;
}clock_format;

clock_format g_StopWatchValue = {0,0,0};

TIMER1_CTC_configurationsType g_Timer1_configurations = {0,15625,TIMER1_FCPU_64};

/*===========================================================================================================
                                      < Private Functions Prototypes >
===========================================================================================================*/



/*===========================================================================================================
                                          < Functions Definitions >
===========================================================================================================*/

void ResetStopWatch(void)
{
	g_StopWatchValue.hours = 0;
	g_StopWatchValue.minutes = 0;
	g_StopWatchValue.seconds = 0;
	TIMER1_CTC_init(&g_Timer1_configurations);
}

void PauseStopWatch(void)
{
	TIMER1_pause();
}

void ResumeStopWatch(void)
{
	TIMER1_resume();
}

void StopWatchTimeProcess(void)
{
	PORTD ^= (1<<7);

	g_StopWatchValue.seconds++;
	if(g_StopWatchValue.seconds == 60)
	{
		g_StopWatchValue.seconds = 0;
		g_StopWatchValue.minutes++;
		if(g_StopWatchValue.minutes == 60)
		{
			g_StopWatchValue.minutes = 0;
			g_StopWatchValue.hours++;
			if(g_StopWatchValue.hours == 99)
				g_StopWatchValue.hours = 0;
		}
	}
}

void StopWatchSystemInit(void)
{
	SREG |= (1<<7);
	INT0_init(INT0_FALLING_EDGE);
	INT1_init(INT1_RISING_EDGE);
	INT2_init(INT2_FALLING_EDGE);
	INT0_setCallBack(ResetStopWatch);
	INT1_setCallBack(PauseStopWatch);
	INT2_setCallBack(ResumeStopWatch);
	TIMER1_setCallBack(StopWatchTimeProcess);
	TIMER1_CTC_init(&g_Timer1_configurations);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+0,OUTPUT_PIN);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+1,OUTPUT_PIN);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+2,OUTPUT_PIN);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+3,OUTPUT_PIN);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+4,OUTPUT_PIN);
	GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+5,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+0,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+1,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+2,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+3,OUTPUT_PIN);

	GPIO_setupPinDirection(PORTD_ID,PIN7_ID,OUTPUT_PIN);
	GPIO_writePin(PORTD_ID,PIN7_ID,HIGH_PIN);
}

/*===========================================================================================================
 * [Function Name] : 
 * [Description]   : 
 * [Arguments]     : <>      -> 
 *                   <>      -> 
 * [return]        : The function returns void.
 ==========================================================================================================*/
int main(void)
{
	StopWatchSystemInit();
	while(1)
	{
		/* Round and display on 7-segments */
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+0,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.seconds)%10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+0,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+1,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.seconds)/10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+1,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+2,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.minutes)%10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+2,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+3,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.minutes)/10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+3,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+4,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.hours)%10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+4,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+5,HIGH_PIN);
		GPIO_writePort(DECODER_PORT_ID,(g_StopWatchValue.hours)/10);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+5,LOW_PIN);
		_delay_ms(DISPLAY_DELAY);

	}
}
