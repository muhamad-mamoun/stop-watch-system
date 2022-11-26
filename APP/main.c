/*
===========================================================================================================
Author       : Mamoun
Date Created : Sep 15, 2022
Project      : Stop Watch System
Description  : System controls the stop-watch time and displays it on 7-segments.
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

#define NUMBER_OF_SELECTIONS       (6u)

#define SELECTION_PORT_ID          (PORTA_ID)
#define FIRST_SELECTION_PIN_ID     (PIN0_ID)

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

/*===========================================================================================================
 * [Function Name] : ResetStopWatch
 * [Description]   : Reset all the stop-watch digits and start count from zero.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ==========================================================================================================*/
void ResetStopWatch(void)
{
	g_StopWatchValue.hours = 0;
	g_StopWatchValue.minutes = 0;
	g_StopWatchValue.seconds = 0;
	TIMER1_CTC_init(&g_Timer1_configurations);
}

/*===========================================================================================================
 * [Function Name] : PauseStopWatch
 * [Description]   : Disable the stop-watch counting.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ==========================================================================================================*/
void PauseStopWatch(void)
{
	TIMER1_pause();
}

/*===========================================================================================================
 * [Function Name] : ResumeStopWatch
 * [Description]   : Take the stop-watch back to continue counting.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ==========================================================================================================*/
void ResumeStopWatch(void)
{
	TIMER1_resume();
}

/*===========================================================================================================
 * [Function Name] : StopWatchTimeProcess
 * [Description]   : Manage the Time process for the stop-watch.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ==========================================================================================================*/
void StopWatchTimeProcess(void)
{
	PORTD ^= (1<<7);  /* Toggle the debugging LED. */

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

/*===========================================================================================================
 * [Function Name] : StopWatchSystemInit
 * [Description]   : Initialize all the stop-watch system modules.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ==========================================================================================================*/
void StopWatchSystemInit(void)
{
	SREG |= (1<<7);             /* Set I-bit to enable the global interrupt. */

	/* Initialize all pins directions. */
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

	/* Activate the internal pull-up resistor supported in AVR. */
	GPIO_writePin(PORTD_ID,PIN2_ID,HIGH_PIN);      /* INT0 pin. */
	GPIO_writePin(PORTB_ID,PIN2_ID,HIGH_PIN);      /* INT2 pin. */

	/* Initialize all system modules. */
	INT0_init(INT0_FALLING_EDGE);
	INT1_init(INT1_RISING_EDGE);
	INT2_init(INT2_FALLING_EDGE);
	INT0_setCallBack(ResetStopWatch);
	INT1_setCallBack(PauseStopWatch);
	INT2_setCallBack(ResumeStopWatch);
	TIMER1_setCallBack(StopWatchTimeProcess);
	TIMER1_CTC_init(&g_Timer1_configurations);

	/* Debugging LED */
	GPIO_setupPinDirection(PORTD_ID,PIN7_ID,OUTPUT_PIN);
	GPIO_writePin(PORTD_ID,PIN7_ID,HIGH_PIN);
}

/*===========================================================================================================
 * [Function Name] : main
 * [Description]   : The main function for the stop-watch system.
 ==========================================================================================================*/
int main(void)
{
	/* Initialize the stop-watch system. */
	StopWatchSystemInit();

	while(1)
	{
		/* Round and display on 7-segments */
		/* 1- Send the required digit to the decoder to be displayed. */
		/* 2- Select the corresponding 7-segments to be activated.    */
		/* 3- Wait a short time to visually perceive.                 */
		/* 4- De-activate the activated 7-segments.                   */
		/* 5- Move to the next digit.                                 */

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.seconds)%10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+0,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+0,LOW_PIN);

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.seconds)/10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+1,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+1,LOW_PIN);

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.minutes)%10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+2,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+2,LOW_PIN);

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.minutes)/10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+3,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+3,LOW_PIN);

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.hours)%10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+4,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+4,LOW_PIN);

		GPIO_writePort(DECODER_PORT_ID,((g_StopWatchValue.hours)/10));
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+5,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+5,LOW_PIN);
	}
}
