/*
===========================================================================================================
Author       : Mamoun
Module       : 7-Segments
File Name    : seven_segments.h
Date Created : Sep 15, 2022
Description  : Header file for the 7-segments driver.
===========================================================================================================
*/


#ifndef HAL_SEVEN_SEGMENTS_H_
#define HAL_SEVEN_SEGMENTS_H_

/*===========================================================================================================
                                               < Includes >
===========================================================================================================*/

#include "../OTHERS/std_types.h"
#include <util/delay.h>

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
                                         < Functions Prototypes >
===========================================================================================================*/



#endif /* HAL_SEVEN_SEGMENTS_H_ */
