/******************************************************************************
 * @file        EXTI.h
 * @author      Mahmoud Karam (ma.karam272@gmail.com)
 * @brief       Interfaces header file for \ref EXTI.c
 * @version     1.0.0
 * @date        2022-03-20
 * @copyright   Copyright (c) 2022
 ******************************************************************************/
#ifndef EXTI_H       
#define EXTI_H   

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                                  TYPEDEFS                                    */
/*                                                                              */
/*------------------------------------------------------------------------------*/

typedef enum{
    FALLING_EDGE,
    RISING_EDGE,
    LOW_LEVEL_DETECT,
    LOGIC_CHANGE
}EXTI_SENSITIVITY_t;

typedef enum{
    EXTI_0,
    EXTI_1,
    EXTI_2,
    NUM_OF_EXTI_CHANNELS
}EXTI_t;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                             API FUNCTIONS PROTOTYPES                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/******************************************************************************
 * @brief       Initialize an EXTI pin with a given sensitivity and callback 
 *              function
 * @param[in]   extiNumber:     The EXTI pin to initialize. See \ref EXTI_t
 * @param[in]   sensitivity:    The sensitivity of the EXTI pin (FALLING_EDGE, 
 *                              RISING_EDGE, LOW_LEVEL_DETECT, LOGIC_CHANGE)
 * @param[in]   callbackPtr:    The callback function to be called when the EXTI 
 *                              pin is triggered
 *****************************************************************************/
void EXTI_Init(const EXTI_t extiNumber, const EXTI_SENSITIVITY_t  sensitivity, void (* const callbackPtr)(void));

/******************************************************************************
 * @brief       Enable an EXTI pin.
 * @param[in]   extiNumber:     The EXTI pin to enable. See \ref EXTI_t
 *****************************************************************************/
void EXTI_EnableExternalInterrupt(const EXTI_t extiNumber);

/******************************************************************************
 * @brief       Disable an EXTI pin. See \ref EXTI_t
 * @param[in]   extiNumber:     The EXTI pin to disable. See \ref EXTI_t
 *****************************************************************************/
void EXTI_DisableExternalInterrupt(const EXTI_t extiNumber);

#endif                  
