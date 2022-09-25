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
 * @date 	23 Sep 2022
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

/*********************************************************************************
 * @brief The states of the system
 ********************************************************************************/
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

/*!< The current state of the system                */
static APP_STATE_t appState = APP_STATE_INIT;

/*!< The current state of the pedestrian button     */
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

/*********************************************************************************
 * @brief   Update the application state
 * @details Update the application state by checking the current state and
 *          calling the corresponding function, then change the state to the next
 *          state
 * @param   void
 * @return  void
 ********************************************************************************/
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

/*********************************************************************************
 * @brief   Notify the application that the button is pressed
 * @details This is a callback used by the EXTI deiver to notify the application 
 *          that the button is pressed by setting the isButtonPressed flag to TRUE
 * @param   void
 * @return  void
 ********************************************************************************/
static void EXTI_Notify(void) {
    isButtonPressed = TRUE;
}

/*********************************************************************************
 * @brief   Cars' green state
 * @details This function is called when the system is in cars' green state, it
 *          will turn on the cars' green light and pedestrian's red ligh, and 
 *          turn off the other lights
 *          Then it will wait for 5 seconds, and if the button is pressed, it will
 *          change the state to pedestrian's init state
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_CarsGreenState(void) {
    /* Configure the pedestrians' light */
    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);
    
    /* Configure the cars' light */
    LED_Set(LED_CAR_G);
    LED_Clr(LED_CAR_Y);
    LED_Clr(LED_CAR_R);

    /* Waiting for the button to be pressed. 
        If the button is pressed, it will go to the next state. 
        If the button is not pressed, it will wait for the next second. 
    */
    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            appState = APP_STATE_PEDESTRIAN_INIT_STATE;
            return;
        }

        TIMER_DelayMs(1000);
    }
}

/*********************************************************************************
 * @brief   Cars' yellow state
 * @details This function is called when the system is in cars' yellow state, it
 *          will blink the cars' yellow light and turn on the pedestrian's red ligh, 
 *          and turn off the other lights
 *          Then it will wait for 5 seconds, and if the button is pressed, it will
 *          change the state to pedestrian's initial state
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_CarsYellowState(void) {
    /* Configure the pedestrians' light */
    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);

    /* Configure the cars' light */
    LED_Set(LED_CAR_Y);
    LED_Set(LED_CAR_G);
    LED_Clr(LED_CAR_R);

    /* Waiting for the button to be pressed, while blinking the yellow light.
        If the button is pressed, it will go to the next state. 
        If the button is not pressed, it will wait for the next second. 
    */
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

/*********************************************************************************
 * @brief   Cars' red state
 * @details This function is called when the system is in cars' red state, it
 *          will turn on the cars' red light and pedestrian's red ligh, and 
 *          turn off the other lights
 *          Then it will wait for 5 seconds, and if the button is pressed, it will
 *          change the state to pedestrian's green state   
 * 
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_CarsRedState(void) {
    /* Configure the pedestrians' light */
    LED_Set(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_Y);
    
    /* Configure the cars' light */
    LED_Set(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Clr(LED_CAR_Y);

    /* Waiting for the button to be pressed. 
        If the button is pressed, it will go to the next state. 
        If the button is not pressed, it will wait for the next second. 
    */
    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        if(isButtonPressed) {
            isButtonPressed = FALSE;
            appState = APP_STATE_PEDESTRIAN_GREEN_STATE;
            return;
        }

        TIMER_DelayMs(1000);
    }
}

/*********************************************************************************
 * @brief   Pedestrian's initial state
 * @details This function is called when the system is in pedestrian's initial state, 
 *          it will turn on the cars' green light and blink both car's yeallo and 
 *          pedestrian's yellow lights, and turn off the other lights
 *          Then it will wait for 5 seconds, and change the state to pedestrian's 
 *          green state. If the button is pressed, it has no effect
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_PedestrianInitState(void) {
    /* Configure the pedestrians' light */
    LED_Clr(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Set(LED_PEDESTRIAN_Y);

    /* Configure the cars' light */
    LED_Clr(LED_CAR_R);
    LED_Set(LED_CAR_G);
    LED_Set(LED_CAR_Y);  

    /* Waiting for the next second, while blinking the yellow lights of
        both cars and pedestrians.
    */
    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {
        TIMER_DelayMs(1000);

        LED_Toggle(LED_PEDESTRIAN_Y);
        LED_Toggle(LED_CAR_Y);
    }


    if(isButtonPressed) {
        isButtonPressed = FALSE;
    }
}

/*********************************************************************************
 * @brief   Pedestrian's green state
 * @details This function is called when the system is in pedestrian's green state, 
 *          it will turn on the cars' red light and pedestrian's green light, and 
 *          turn off the other lights
 *          Then it will wait for 5 seconds, and change the state to pedestrian's 
 *          final state. If the button is pressed, it has no effect
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_PedestrianGreenState(void) {
    /* Configure the pedestrians' light */
    LED_Set(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Clr(LED_PEDESTRIAN_Y);

    /* Configure the cars' light */
    LED_Set(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Clr(LED_CAR_Y);


    TIMER_DelayMs(5000);
    if(isButtonPressed) {
        isButtonPressed = FALSE;
    }
}

/*********************************************************************************
 * @brief   Pedestrian's final state
 * @details This function is called when the system is in pedestrian's final state, 
 *          it will turn on the cars' red light and blink both car's yeallo and 
 *          pedestrian's yellow lights, and turn off the other lights
 *          Then it will wait for 5 seconds, and change the state to cars' green 
 *          state. 
 *          If the button is pressed, it has no effect
 * @param   void
 * @return  void
 ********************************************************************************/
static void APP_PedestrianFinalState(void) {
    /* Configure the pedestrians' light */
    LED_Set(LED_PEDESTRIAN_G);
    LED_Clr(LED_PEDESTRIAN_R);
    LED_Set(LED_PEDESTRIAN_Y);

    /* Configure the cars' light */
    LED_Clr(LED_CAR_R);
    LED_Clr(LED_CAR_G);
    LED_Set(LED_CAR_Y);

    /* Waiting for the next second, while blinking the yellow lights of
        both cars and pedestrians.
    */
    for(u8_t i = 0; i < STATE_TIME_SEC; i++) {        
        TIMER_DelayMs(1000);

        LED_Toggle(LED_PEDESTRIAN_Y);
        LED_Toggle(LED_CAR_Y);
    }


    if(isButtonPressed) {
        isButtonPressed = FALSE;
    }
}

