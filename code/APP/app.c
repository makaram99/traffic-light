/*********************************************************************************
 * @file 	app.c
 * @author 	Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief 	Simple Traffic Light System with Pedeestrian Button
 * @details The system is in normal mode, and works as a traffic light system:
 *              * Green light for 5 seconds
 *              * Yellow light for 5 seconds
 *              * Red light for 5 seconds
 *              * Yellow light for 5 seconds
 *              * Repeat
 *          If the pedestrian button is pressed, the system will be in pedestrian
 *          mode:
 *              * If the button is pressed while the cars are in RED light, the
 *                  pedestrian light will be GREEN and cars' light will be RED 
 *                  for 5 seconds
 *              * If the button is pressed while the cars are in GREEN light, or 
 *                  YELLOW is blinking, the pedestrian light will be RED, and both 
 *                  cars' and pedestrian's lights will be YELLOW blinking for 5 
 *                  seconds, then the pedestrian light will be GREEN and cars'
 *                  light will be RED for 5 seconds
 *              At the end of both states, the system will be in normal mode
 *
 * @version 1.0.0
 * @date 	2022-07-02
 * @copyright Mahmoud Karam Emara 2022, MIT License
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                                  INCLUDES                                    */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/DIO/DIO.h"
#include "../MCAL/EXTI/EXTI.h"
#include "../MCAL/TIMER/TIMER.h"

#include "../HAL/LED/LED.h"
#include "../HAL/BUTTON/BUTTON.h"

#include "app.h"
#include "app_cfg.h"

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                        PRIVATE FUNCTIONS PROTOTYPES                          */
/*                                                                              */
/*------------------------------------------------------------------------------*/

static void APP_UpdateState(void);
static void EXTI_Notify(void);
static void APP_CarsGreenState(void);
static void APP_CarsYellowState(void);
static void APP_CarsRedState(void);
static void APP_PedestrianInitState(void);
static void APP_PedestrianGreenState(void);
static void APP_PedestrianFinalState(void);


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                                  TYPEDEFS                                    */
/*                                                                              */
/*------------------------------------------------------------------------------*/

typedef enum {
    APP_STATE_INIT,

    APP_STATE_CARS_GREEN,
    APP_STATE_CARS_YELLOW,
    APP_STATE_CARS_RED,
    
    APP_STATE_PEDESTRIAN_INIT_STATE,
    APP_STATE_PEDESTRIAN_GREEN_STATE,
    APP_STATE_PEDESTRIAN_FINAL_STATE,

} APP_STATE_t;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                           PRIVATE GLOBALS VARIABLES                          */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static APP_STATE_t appState = APP_STATE_INIT;
static BOOL_t isButtonPressed = FALSE;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                             PUBLIC FUNCTIONS                                 */
/*                                                                              */
/*------------------------------------------------------------------------------*/

void APP_Init(void) {
    appState = APP_STATE_INIT;

    DIO_Init();
    EXTI_Init(EXTI_0, FALLING_EDGE, EXTI_Notify);
    LED_Init();
    BUTTON_Init();

    EXTI_EnableExternalInterrupt(EXTI_0);
}

void APP_Start(void) {
    APP_UpdateState();

    while(1) {
		APP_UpdateState();
	}

	return;
}



/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                             PRIVATE FUNCTIONS                                */
/*                                                                              */
/*------------------------------------------------------------------------------*/

static void APP_UpdateState(void) {
    switch(appState) {
        case APP_STATE_INIT:
            appState = APP_STATE_CARS_GREEN;
            break;
        case APP_STATE_CARS_GREEN:
            appState = APP_STATE_CARS_YELLOW;
            APP_CarsGreenState();
            break;
        case APP_STATE_CARS_YELLOW:
            appState = APP_STATE_CARS_RED;
            APP_CarsYellowState();
            break;
        case APP_STATE_CARS_RED:
            appState = APP_STATE_CARS_GREEN;
            APP_CarsRedState();
            break;
        case APP_STATE_PEDESTRIAN_INIT_STATE:
            appState = APP_STATE_PEDESTRIAN_GREEN_STATE;
            APP_PedestrianInitState();
            break;
        case APP_STATE_PEDESTRIAN_GREEN_STATE:
            appState = APP_STATE_PEDESTRIAN_FINAL_STATE;
            APP_PedestrianGreenState();
            break;
        case APP_STATE_PEDESTRIAN_FINAL_STATE:
            appState = APP_STATE_CARS_GREEN;
            APP_PedestrianFinalState();
            break;
        default:
            break;
    }
}

static void EXTI_Notify(void) {
    isButtonPressed = TRUE;
}

static void APP_CarsGreenState(void) {
    LED_Set(LED_CAR_G);
    LED_Clr(LED_CAR_Y);
    LED_Clr(LED_CAR_R);

    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            appState = APP_STATE_PEDESTRIAN_INIT_STATE;
            return;
        }

        TIMER_DelayMs(1000);
    }
}

static void APP_CarsYellowState(void) {
    LED_Set(LED_CAR_Y);
    LED_Set(LED_CAR_G);
    LED_Clr(LED_CAR_R);

    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            appState = APP_STATE_PEDESTRIAN_INIT_STATE;
            return;
        }

        TIMER_DelayMs(1000);

        LED_Toggle(LED_CAR_Y);
    }
}

static void APP_CarsRedState(void) {
    LED_Set(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Clr(LED_CAR_Y);

    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            appState = APP_STATE_PEDESTRIAN_GREEN_STATE;
            return;
        }

        TIMER_DelayMs(1000);
    }
}

static void APP_PedestrianInitState(void) {
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Set(LED_PEDESTRIAN_Y);

    LED_Clr(LED_CAR_R);
    LED_Set(LED_CAR_G);
    LED_Set(LED_CAR_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            /* Do nothing: remain in the same state */
        }
        TIMER_DelayMs(1000);

        LED_Toggle(LED_PEDESTRIAN_Y);
        LED_Toggle(LED_CAR_Y);
    }
}

static void APP_PedestrianGreenState(void) {
    LED_Set(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_Y);

    LED_Set(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Clr(LED_CAR_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            /* Do nothing: remain in the same state */
        }
        TIMER_DelayMs(1000);
    }
}

static void APP_PedestrianFinalState(void) {
    LED_Set(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Set(LED_PEDESTRIAN_Y);

    LED_Clr(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Set(LED_CAR_Y);

    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            /* Do nothing: remain in the same state */
        }
        TIMER_DelayMs(1000);

        LED_Toggle(LED_PEDESTRIAN_Y);
        LED_Toggle(LED_CAR_Y);
    }
}


