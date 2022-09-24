// /*********************************************************************************
//  * @file    driverTest.c
//  * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
//  * @brief   This file contains the drivers test implementation.
//  * @details This module is used to test the drivers functionality. 
//  *          It is not part of the application.
//  *          To test application units, uncomment the corresponding line in the
//  *          main function. UNCOMMENT ONLY ONE LINE AT A TIME.
//  * 
//  * @version 1.0.0
//  * @date    23 Sep 2022
//  * 
//  * @copyright Copyright (c) 2022
//  * 
//  ********************************************************************************/

// /*------------------------------------------------------------------------------*/
// /*                                                                              */
// /*                                  INCLUDES                                    */
// /*                                                                              */
// /*------------------------------------------------------------------------------*/

// #include "../LIB/STD_TYPES.h"
// #include "../LIB/BIT_MATH.h"

// #include "../MCAL/DIO/DIO.h"
// #include "../MCAL/EXTI/EXTI.h"
// #include "../MCAL/TIMER/TIMER.h"

// #include "../HAL/LED/LED.h"
// #include "../HAL/BUTTON/BUTTON.h"

// #include <util/delay.h>


// /*-----------------------------------------------------------------------------*/
// /*                                                                             */
// /*                            TEST FUNCTION PROTOTYPES                         */
// /*                                                                             */
// /*-----------------------------------------------------------------------------*/

// static void test_DIO(void);
// static void test_EXTI(void);
// static void test_LED(void);
// static void test_BUTTON(void);
// static void test_TIMER(void);

// static void EXTI_Notify(void);


// /*-----------------------------------------------------------------------------*/
// /*                                                                             */
// /*                                     MAIN                                    */
// /*                                                                             */
// /*-----------------------------------------------------------------------------*/

// int main(void) {

//     #if 0       /* Test DIO */
//     DIO_Init();
//     test_DIO();

//     #elif 0     /* Test EXTI */
//     DIO_Init();
//     test_EXTI();

//     #elif 0     /* Test LED */
//     DIO_Init();
//     LED_Init();
//     test_LED();

//     #elif 0     /* Test BUTTON */
//     DIO_Init();
//     BUTTON_Init();
//     test_BUTTON();

//     #elif 1     /* Test TIMER */
//     DIO_Init();
//     LED_Init();
//     test_TIMER();
//     #endif

//     while(1) {

//     }

//     return 0;
// }


// /*-----------------------------------------------------------------------------*/
// /*                                                                             */
// /*                            TEST FUNCTION DEFINITIONS                        */
// /*                                                                             */
// /*-----------------------------------------------------------------------------*/

// static void test_DIO(void) {
//     #if 1       /* Test writing functions: there are 6 blinks i theis test  */

//     /* Testing DIO_SetPinValue function */
//     DIO_SetPinValue(DIO_PINS_CAR_LED_R, HIGH);
//     DIO_SetPinValue(DIO_PINS_CAR_LED_Y, HIGH);
//     DIO_SetPinValue(DIO_PINS_CAR_LED_G, HIGH);
//     _delay_ms(1000);
//     DIO_SetPinValue(DIO_PINS_CAR_LED_R, LOW);
//     DIO_SetPinValue(DIO_PINS_CAR_LED_Y, LOW);
//     DIO_SetPinValue(DIO_PINS_CAR_LED_G, LOW);
//     _delay_ms(1000);

//     /* Testing Set/Clr functions */
//     DIO_SetPin(DIO_PINS_CAR_LED_R);
//     DIO_SetPin(DIO_PINS_CAR_LED_Y);
//     DIO_SetPin(DIO_PINS_CAR_LED_G);
//     _delay_ms(1000);
//     DIO_ClrPin(DIO_PINS_CAR_LED_R);
//     DIO_ClrPin(DIO_PINS_CAR_LED_Y);
//     DIO_ClrPin(DIO_PINS_CAR_LED_G);
//     _delay_ms(1000);

//     /* Testing DIO_SetPortValue function */
//     DIO_SetPortValue(DIO_PINS_CAR_LED_R, 0x07);
//     _delay_ms(1000);
//     DIO_SetPortValue(DIO_PINS_CAR_LED_R, 0xf8);
//     _delay_ms(1000);
//     DIO_SetPortValue(DIO_PINS_CAR_LED_R, 0x00);
//     _delay_ms(1000);

//     /* Testing DIO_TooglePin function */
//     DIO_TogglePin(DIO_PINS_CAR_LED_R);
//     DIO_TogglePin(DIO_PINS_CAR_LED_Y);
//     DIO_TogglePin(DIO_PINS_CAR_LED_G);
//     _delay_ms(1000);
//     DIO_TogglePin(DIO_PINS_CAR_LED_R);
//     DIO_TogglePin(DIO_PINS_CAR_LED_Y);
//     DIO_TogglePin(DIO_PINS_CAR_LED_G);
//     _delay_ms(1000);

//     /* Testing DIO_TogglePort function */
//     DIO_TogglePort(DIO_PINS_CAR_LED_R);
//     _delay_ms(1000);
//     DIO_TogglePort(DIO_PINS_CAR_LED_R);
//     _delay_ms(1000);

//     #else   /* Test read function           */
//     while(1) {
//         /* Testing DIO_ReadPin function */
//         uint8_t pinValue = 0;
//         DIO_ReadPin(DIO_PINS_PEDESTRIAN_BUTTON, &pinValue);
//         DIO_SetPinValue(DIO_PINS_CAR_LED_R, !pinValue);
//     } 

//     #endif
// }

// static void test_EXTI(void) {
    
//     #if 0       /* Test EXTI Falling Edge */
//     EXTI_Init(EXTI_0, FALLING_EDGE, EXTI_Notify);

//     #elif 0     /* Test EXTI Rising Edge */
//     EXTI_Init(EXTI_0, RISING_EDGE, EXTI_Notify);

//     #elif 0     /* Test EXTI On Change */
//     EXTI_Init(EXTI_0, LOGIC_CHANGE, EXTI_Notify);

//     #elif 1     /* Test EXTI Low Level */
//     EXTI_Init(EXTI_0, LOW_LEVEL_DETECT, EXTI_Notify);
//     #endif

//     EXTI_EnableExternalInterrupt(EXTI_0);
// }

// static void test_LED(void) {
    
//     #if 0       /* Test writing functions: there are 6 blinks i theis test  */

//     /* Testing LED_SetClr function */
//     LED_SetClr(LED_CAR_R, HIGH);
//     LED_SetClr(LED_CAR_Y, HIGH);
//     LED_SetClr(LED_CAR_G, HIGH);
//     _delay_ms(1000);
//     LED_SetClr(LED_CAR_R, LOW);
//     LED_SetClr(LED_CAR_Y, LOW);
//     LED_SetClr(LED_CAR_G, LOW);
//     _delay_ms(1000);

//     /* Testing Set/Clr functions */
//     LED_Set(LED_CAR_R);
//     LED_Set(LED_CAR_Y);
//     LED_Set(LED_CAR_G);
//     _delay_ms(1000);
//     LED_Clr(LED_CAR_R);
//     LED_Clr(LED_CAR_Y);
//     LED_Clr(LED_CAR_G);
//     _delay_ms(1000);

//     /* Testing LED_Toogle function */
//     LED_Toggle(LED_CAR_R);
//     LED_Toggle(LED_CAR_Y);
//     LED_Toggle(LED_CAR_G);
//     _delay_ms(1000);
//     LED_Toggle(LED_CAR_R);
//     LED_Toggle(LED_CAR_Y);
//     LED_Toggle(LED_CAR_G);
//     _delay_ms(1000);

//     #else   /* Test read function           */
//     while(1) {
//         /* Testing LED_ReadPin function */
//         uint8_t pinValue = 0;
//         DIO_ReadPin(DIO_PINS_PEDESTRIAN_BUTTON, &pinValue);
//         LED_SetClr(LED_CAR_R, !pinValue);

//         LED_Read(LED_CAR_R, &pinValue);
//         LED_SetClr(LED_CAR_G, pinValue);
//     } 

//     #endif

// }

// static void test_BUTTON(void) {
//     while(1) {
//         /* Testing BUTTON_Read function */
//         uint8_t pinValue = 0;
//         BUTTON_Read(BUTTON_PEDESTRIAN, &pinValue);
//         LED_SetClr(LED_CAR_R, pinValue);
//     } 

// }

// static void test_TIMER(void) {

//     /* Test 1 Second delay */
//     LED_Set(LED_CAR_R);
//     TIMER_DelayMs(1000);
//     LED_Clr(LED_CAR_R);
//     TIMER_DelayMs(1000);

//     /* Test 2 Second delay */
//     LED_Set(LED_CAR_R);
//     TIMER_DelayMs(2000);
//     LED_Clr(LED_CAR_R);
//     TIMER_DelayMs(2000);

//     /* Test 5 Second delay */
//     LED_Set(LED_CAR_R);
//     TIMER_DelayMs(5000);
//     LED_Clr(LED_CAR_R);
//     TIMER_DelayMs(5000);

//     LED_Set(LED_CAR_R);
// }


// static void EXTI_Notify(void) {
//     DIO_TogglePort(DIO_PINS_CAR_LED_R);
// }
