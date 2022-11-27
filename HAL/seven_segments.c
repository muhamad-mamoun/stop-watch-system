/*
===========================================================================================================
Author       : Mamoun
Module       : 7-Segments
File Name    : seven_segments.c
Date Created : Sep 15, 2022
Description  : Source file for the 7-segments driver.
===========================================================================================================
*/

#warning "The seven segments driver is not finished yet."

/*===========================================================================================================
                                               < Includes >
===========================================================================================================*/

#include "seven_segments.h"
#include "../MCAL/gpio.h"

/*===========================================================================================================
                                           < Global Variables >
===========================================================================================================*/



/*===========================================================================================================
                                      < Private Functions Prototypes >
===========================================================================================================*/



/*===========================================================================================================
                                          < Functions Definitions >
===========================================================================================================*/

/*===========================================================================================================
 * [Function Name] : 
 * [Description]   : 
 * [Arguments]     : <>      -> 
 *                   <>      -> 
 * [return]        : The function returns void.
 ==========================================================================================================*/
void SEVENSEGMENTS_init(void)
{
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+0,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+1,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+2,OUTPUT_PIN);
	GPIO_setupPinDirection(DECODER_PORT_ID,FIRST_DECODER_PIN_ID+3,OUTPUT_PIN);

	for(uint8 counter=0; counter<NUMBER_OF_SELECTIONS; counter++)
	{
		GPIO_setupPinDirection(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+counter,OUTPUT_PIN);
	}
}

void SEVENSEGMENTS_display(uint8 a_sevenSegmentsID, uint8 a_value)
{
	if(a_sevenSegmentsID < NUMBER_OF_SELECTIONS)
	{
		GPIO_writePort(DECODER_PORT_ID,a_value);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+a_sevenSegmentsID,HIGH_PIN);
		_delay_ms(DISPLAY_DELAY);
		GPIO_writePin(SELECTION_PORT_ID,FIRST_SELECTION_PIN_ID+a_sevenSegmentsID,LOW_PIN);
	}
}
