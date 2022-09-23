/******************************************************************************
 * @file        LED_cfg.c
 * @author      Mahmoud Karam (ma.karam272@gmail.com)
 * @brief       Configuration source file for \ref LED.c
 * @version     1.0.0
 * @date        2022-03-20
 * @copyright   Copyright (c) 2022
 ******************************************************************************/
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/DIO/DIO.h"
#include "LED.h"
#include "LED_cfg.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                     CHANGE THE FOLLOWING TO YOUR NEEDS                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

LED_CONFIGS_t ledConfigs[] = {
    {LED_CAR_R, DIO_PINS_CAR_LED_R},
    {LED_CAR_Y, DIO_PINS_CAR_LED_Y},
    {LED_CAR_G, DIO_PINS_CAR_LED_G},

    {LED_PEDESTRIAN_R, DIO_PINS_PEDESTRIAN_LED_R},
    {LED_PEDESTRIAN_Y, DIO_PINS_PEDESTRIAN_LED_Y},
    {LED_PEDESTRIAN_G, DIO_PINS_PEDESTRIAN_LED_G},
};

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                     DO NOT CHANGE ANYTHING BELOW THIS LINE                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const u8_t countLedsConfigured = sizeof(ledConfigs) / sizeof(ledConfigs[0]);