/**************************************************************************
 * @file        TIMER.c
 * @author      Mahmoud Karam (ma.karam272@gmail.com)
 * @brief       Timer driver for Atmega32 microcontroller.
 * @version     1.0.0
 * @date        2022-02-11
 * @copyright   Copyright (c) 2022
 ***************************************************************************/
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../GIE/GIE.h"

#include "TIMER_reg.h"
#include "TIMER.h"


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                          CALLBACKS POINTERS                                  */
/*                                                                              */                                
/*------------------------------------------------------------------------------*/

static void (*TIMER0_OVF_CBK_PTR)(void)  ;
static void (*TIMER0_COMP_CBK_PTR)(void) ;
 
static void (*TIMER1_OVF_CBK_PTR)(void)  ;
static void (*TIMER1_COMPA_CBK_PTR)(void);
static void (*TIMER1_COMPB_CBK_PTR)(void);
static void (*TIMER1_CAPT_CBK_PTR)(void) ;
 
static void (*TIMER2_OVF_CBK_PTR)(void)  ;
static void (*TIMER2_COMP_CBK_PTR)(void) ;


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                       PRIVATE FUNCTIONS DECLARATIONS                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static void TIMER_SetCallBack(void (**destinationCallback)(void), void (*sourceCallback)(void) );
static void TIMER0_ConfigClock(const TIMER_CLOCK_t clock);
static void TIMER0_ConfigMode(const TIMER_MODE_t timerMode);
static void TIMER0_ConfigOC(const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode);
static void TIMER1_ConfigOC(const TIMER_OCx_t OCx, const TIMER_OC_t compareMode);
static void TIMER1_ConfigClock(const TIMER_CLOCK_t timerClock);
static void TIMER1_ConfigMode(const TIMER_MODE_t timerMode);
static void TIMER2_ConfigClock(const TIMER_CLOCK_t clock);
static void TIMER2_ConfigMode(const TIMER_MODE_t timerMode);
static void TIMER2_ConfigOC(const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          PUBLIC FUNCTIONS OF TIMER0                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

ERROR_t TIMER_DelayMs(const u64_t periodInMs) {
    u32_t overflowCounter = 0;
    u32_t overflowCounterMax = 0;

    f64_t maxDelayInMs = ((f32_t)1 / F_CPU) * ((f32_t)TIMER0_GetTop() + 1) * 1000;
    overflowCounterMax = (f32_t)periodInMs / maxDelayInMs;

    TIMER0_Init(0, F_CPU_CLOCK, TIMER_MODE_NORMAL, NO_OC);

    while(overflowCounter < overflowCounterMax) {
        while(BIT_IS_CLEAR(TIMER_u8_tTIFR_REG, TOV0)) {
            /* Wait for overflow */
        }

        /* Clear overflow flag */
        BIT_SET(TIMER_u8_tTIFR_REG, TOV0);

        overflowCounter++;
    }

    TIMER0_Disable();

    return ERROR_OK;
}

void TIMER0_Init(const u8_t u8InitValue, const TIMER_CLOCK_t clock, 
                 const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode) {
    TIMER0_SetTimer(u8InitValue);
    TIMER0_ConfigClock(clock);
    TIMER0_ConfigMode(timerMode);
    TIMER0_ConfigOC(timerMode, compareMode);
}

void TIMER0_Disable(void) {
    TIMER0_ConfigClock(NO_CLOCK);
    TIMER0_ConfigOC(TIMER_MODE_NORMAL, NO_OC);
}

void TIMER0_SetCompareValue(const u8_t u8CompareValue) {
    OCR0 = u8CompareValue;
}

void TIMER0_SetTimer(const u8_t u8TimerValue) {
    TCNT0 = u8TimerValue;
}

void TIMER0_EnableOverflowInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER0_OVF_CBK_PTR, callback);

    /* Enable the overflow interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, TOIE0);

    GIE_Enable();
}

void TIMER0_DisableOverflowInterrupt(void) {
    BIT_CLR(TIMER_u8_tTIMSK_REG, TOIE0);
}

void TIMER0_EnableCompareMatchInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER0_COMP_CBK_PTR, callback);

    /* Enable the compare match interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, OCIE0);

    GIE_Enable();
}

void TIMER0_DisableCompareMatchInterrupt(void) {
    BIT_CLR(TIMER_u8_tTIMSK_REG, OCIE0);
}

u8_t TIMER0_GetTimerValue(void) {
    return TCNT0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          PUBLIC FUNCTIONS OF TIMER1                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void TIMER1_Init(const u16_t u16InitValue, const TIMER_CLOCK_t clock, 
                 const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode, 
                 const TIMER_OCx_t OCx) {

    TIMER1_SetTimer(u16InitValue);
    TIMER1_ConfigClock(clock);
    TIMER1_ConfigMode(timerMode);
    TIMER1_ConfigOC(OCx, compareMode);  
}

void TIMER1_Disable(const TIMER_OCx_t OCx) {
    TIMER1_ConfigClock(NO_CLOCK);
    TIMER1_ConfigOC(OCx, NO_OC);
}

void TIMER1_SetCompareValue(const u16_t u16CompareValue, const TIMER_OCx_t OCx) {
    GIE_Disable();

    switch(OCx) {
        case TIMER_OCA:
            OCR1AH = (const u8_t)(u16CompareValue >> 8);
            OCR1AL = (const u8_t)(u16CompareValue);
            break;
        case TIMER_OCB:
            OCR1BH = (const u8_t)(u16CompareValue >> 8);
            OCR1BL = (const u8_t)(u16CompareValue);
            break;
        default:
            break;
    }

    GIE_Enable();
}

void TIMER1_SetTimer(const u16_t u16TimerValue) {
    GIE_Disable();

    /* Upper register must be written first */
    TCNT1H = (u8_t)(u16TimerValue >> 8);
    TCNT1L = (u8_t)(u16TimerValue);

    GIE_Enable();
}

void TIMER1_EnableOverflowInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER1_OVF_CBK_PTR, callback);

    /* Enable the overflow interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, TOIE1);

    GIE_Enable();
}

void TIMER1_DisableOverflowInterrupt(void) {
    BIT_CLR(TIMER_u8_tTIMSK_REG, TOIE1);  /*!< Disable the overflow interrupt */
}

void TIMER1_EnableCompareMatchInterrupt(const TIMER_OCx_t OCx, void (* const callback)(void)) {
    GIE_Disable();

    switch(OCx) {
        case TIMER_OCA:
            TIMER_SetCallBack(&TIMER1_COMPA_CBK_PTR, callback);
            BIT_SET(TIMER_u8_tTIMSK_REG, OCIE1A);
            break;
        case TIMER_OCB:
            TIMER_SetCallBack(&TIMER1_COMPB_CBK_PTR, callback);
            BIT_SET(TIMER_u8_tTIMSK_REG, OCIE1B);
            break;
        default:
            break;
    }    

    GIE_Enable();
}

void TIMER1_DisableCompareMatchInterrupt(const TIMER_OCx_t OCx) {
    switch(OCx) {
        case TIMER_OCA:
            BIT_CLR(TIMER_u8_tTIMSK_REG, OCIE1A);
            break;
        case TIMER_OCB:
            BIT_CLR(TIMER_u8_tTIMSK_REG, OCIE1B);
            break;
        default:
            break;
    }
}

void TIMER1_EnableCaptureInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER1_CAPT_CBK_PTR, callback);

    /* Enable the capture interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, TICIE1);

    GIE_Enable();
}

void TIMER1_DisableCaptureInterrupt(void) {
    GIE_Disable();

    /* Disable the capture interrupt */
    BIT_CLR(TIMER_u8_tTIMSK_REG, TICIE1);

    GIE_Enable();
}

u16_t TIMER1_GetTimerValue(void) {
    u16_t u16TimerValue = 0;

    GIE_Disable();

    /* Lower register must be read first */
    u16TimerValue = (u16_t)TCNT1L;
    u16TimerValue |= (u16_t)(TCNT1H << 8);

    GIE_Enable();

    return (u16TimerValue);
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          PUBLIC FUNCTIONS OF TIMER0                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void TIMER2_Init(const u8_t u8InitValue, const TIMER_CLOCK_t clock, 
                 const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode) {
    TIMER2_SetTimer(u8InitValue);
    TIMER2_ConfigClock(clock);
    TIMER2_ConfigMode(timerMode);
    TIMER2_ConfigOC(timerMode, compareMode);
}

void TIMER2_Disable(void) {
    TIMER2_ConfigClock(NO_CLOCK);
    TIMER2_ConfigOC(TIMER_MODE_NORMAL, NO_OC);
}

void TIMER2_SetCompareValue(const u8_t u8CompareValue) {
    OCR2 = u8CompareValue;
}

void TIMER2_SetTimer(const u8_t u8TimerValue) {
    TCNT2 = u8TimerValue;
}

void TIMER2_EnableOverflowInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER2_OVF_CBK_PTR, callback);

    /* Enable the overflow interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, TOIE2);

    GIE_Enable();
}

void TIMER2_DisableOverflowInterrupt(void) {
    BIT_CLR(TIMER_u8_tTIMSK_REG, TOIE2);
}

void TIMER2_EnableCompareMatchInterrupt(void (* const callback)(void)) {
    GIE_Disable();

    TIMER_SetCallBack(&TIMER2_COMP_CBK_PTR, callback);

    /* Enable the compare match interrupt */
    BIT_SET(TIMER_u8_tTIMSK_REG, OCIE2);

    GIE_Enable();
}

void TIMER2_DisableCompareMatchInterrupt(void) {
    BIT_CLR(TIMER_u8_tTIMSK_REG, OCIE2);
}

u8_t TIMER2_GetTimerValue(void) {
    return (u8_t)TCNT2;
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              PWM FUNCTIONS                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void PWM_Init(const PWM_t channel, const u32_t u32Frequency) {
    TIMER_CLOCK_t clock;
    u16_t prescaler = 0;
    
    prescaler = F_CPU / ((u32_t)256 * u32Frequency);

    if(prescaler / 1024) {
        clock = F_CPU_1024;
    }else if(prescaler / 256) {
        clock = F_CPU_256;
    }else if(prescaler / 64) {
        clock = F_CPU_64;
    }else if(prescaler / 8) {
        clock = F_CPU_8;
    }else{
        clock = F_CPU_CLOCK;
    }

    switch(channel) {
        case PWM_0:
            TIMER0_Init(0, clock, TIMER_MODE_FAST_PWM, CLEAR_OC);
            break;
        case PWM_1:
            TIMER1_Init(0, clock, TIMER_MODE_FAST_PWM_8, CLEAR_OC, TIMER_OCA);
            break;
        case PWM_2:
            TIMER1_Init(0, clock, TIMER_MODE_FAST_PWM_8, CLEAR_OC, TIMER_OCB);
            break;
        case PWM_3:
            TIMER2_Init(0, clock, TIMER_MODE_FAST_PWM, CLEAR_OC);
            break;
        /* TODO: implement cases for Timer 2 and 3  */
        default:
            /* DEBUG    */
            break;
    }
}

void PWM_Write(const PWM_t channel, const u8_t u8DutyCyclePercentage) {
    u16_t u16_tOcr = 0;
    u16_t u16_tTop = 0;

    switch(channel) {
        case PWM_0:
            u16_tTop = TIMER0_GetTop();
            u16_tOcr = (u16_t)( ((u32_t)u16_tTop * u8DutyCyclePercentage) / 100 );
            TIMER0_SetCompareValue(u16_tOcr);
            break;
        case PWM_1:
            u16_tTop = TIMER1_GetTop();
            u16_tOcr = (u16_t)( ((u32_t)u16_tTop * u8DutyCyclePercentage) / 100 );
            TIMER1_SetCompareValue(u16_tOcr, TIMER_OCA);
            break;
        case PWM_2:
            u16_tTop = TIMER1_GetTop();
            u16_tOcr = (u16_t)( ((u32_t)u16_tTop * u8DutyCyclePercentage) / 100 );
            TIMER1_SetCompareValue(u16_tOcr, TIMER_OCB);
            break;
        case PWM_3:
            u16_tTop = TIMER2_GetTop();
            u16_tOcr = (u16_t)( ((u32_t)u16_tTop * u8DutyCyclePercentage) / 100 );
            TIMER2_SetCompareValue(u16_tOcr);
            break;
        default:
            /* DEBUG    */
            break;
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   PRIVATE FUNCTIONS (GENERIC)                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

static void TIMER_SetCallBack(void (**destinationCallback)(void), 
                              void (*sourceCallback)(void) ) {
    if( (sourceCallback != NULL) && (destinationCallback != NULL) ) {
        *destinationCallback = sourceCallback;
    } else {
        /* TODO: Handle error */
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   PRIVATE FUNCTIONS OF TIMER0                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

static void TIMER0_ConfigClock(const TIMER_CLOCK_t clock) {
    switch(clock) {
        case NO_CLOCK:
            BIT_CLR(TCCR0, CS00);
            BIT_CLR(TCCR0, CS01);
            BIT_CLR(TCCR0, CS02);
            break;
        case F_CPU_CLOCK:
            BIT_SET(TCCR0, CS00);
            BIT_CLR(TCCR0, CS01);
            BIT_CLR(TCCR0, CS02);
            break;
        case F_CPU_8:
            BIT_CLR(TCCR0, CS00);
            BIT_SET(TCCR0, CS01);
            BIT_CLR(TCCR0, CS02);
            break;
        case F_CPU_64:
            BIT_SET(TCCR0, CS00);
            BIT_SET(TCCR0, CS01);
            BIT_CLR(TCCR0, CS02);
            break;
        case F_CPU_256:
            BIT_CLR(TCCR0, CS00);
            BIT_CLR(TCCR0, CS01);
            BIT_SET(TCCR0, CS02);
            break;
        case F_CPU_1024:
            BIT_SET(TCCR0, CS00);
            BIT_CLR(TCCR0, CS01);
            BIT_SET(TCCR0, CS02);
            break;
        case F_CPU_EXT_CLK_FALLING:
            BIT_CLR(TCCR0, CS00);
            BIT_SET(TCCR0, CS01);
            BIT_SET(TCCR0, CS02);
            break;
        case F_CPU_EXT_CLK_RISING:
            BIT_SET(TCCR0, CS00);
            BIT_SET(TCCR0, CS01);
            BIT_SET(TCCR0, CS02);
            break;
        default:
            /* TODO: DEBUG    */
            break;
    }
}

static void TIMER0_ConfigMode(const TIMER_MODE_t timerMode) {
    switch(timerMode) {
        case TIMER_MODE_NORMAL:       
            BIT_CLR(TCCR0, WGM00);
            BIT_CLR(TCCR0, WGM01);
            BIT_SET(TCCR0, FOC0);   /*!< Set force output compare mode with non-PWM mode */
            break;
        case TIMER_MODE_CTC:         
            BIT_CLR(TCCR0, WGM00);
            BIT_SET(TCCR0, WGM01);
            BIT_SET(TCCR0, FOC0);   /*!< Set force output compare mode with non-PWM mode */
            break;
        case TIMER_MODE_FAST_PWM:   
            BIT_SET(TCCR0, WGM00);
            BIT_SET(TCCR0, WGM01);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM:
            BIT_SET(TCCR0, WGM00);
            BIT_CLR(TCCR0, WGM01);
            break;
        default:                
            /* TODO: DEBUG    */
            break;
    }
}

static void TIMER0_ConfigOC(const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode) {
    switch(compareMode) {
        case NO_OC:
            BIT_CLR(TCCR0, COM00);
            BIT_CLR(TCCR0, COM01);
            break;
        case TOGGLE_OC:
            if((timerMode == TIMER_MODE_NORMAL) || (timerMode == TIMER_MODE_CTC)) {
                BIT_SET(TCCR0, COM00);
                BIT_CLR(TCCR0, COM01);
            }
            else{
                /* TODO: DEBUG    */
            }
            break;
        case CLEAR_OC:
            BIT_CLR(TCCR0, COM00);
            BIT_SET(TCCR0, COM01);
            break;
        case SET_OC:  
            BIT_SET(TCCR0, COM00);
            BIT_SET(TCCR0, COM01);
            break;
        default:
            /* TODO: DEBUG    */
            break;
    }
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   PRIVATE FUNCTIONS OF TIMER1                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

static void TIMER1_ConfigOC(const TIMER_OCx_t OCx, const TIMER_OC_t compareMode) {
    u8_t u8_tCom1x0 = 0, u8_tCom1x1 = 0;

    switch(OCx) {
        case TIMER_OCA:
            u8_tCom1x0 = COM1A0;
            u8_tCom1x1 = COM1A1;
            break;
        case TIMER_OCB:
            u8_tCom1x0 = COM1B0;
            u8_tCom1x1 = COM1B1;
            break;
        default:
            break;
    }

    switch(compareMode) {
        case NO_OC:
            BIT_CLR(TCCR1A, u8_tCom1x0);
            BIT_CLR(TCCR1A, u8_tCom1x1);
            break;
        case TOGGLE_OC:
            BIT_SET(TCCR1A, u8_tCom1x0);
            BIT_CLR(TCCR1A, u8_tCom1x1);
            break;
        case CLEAR_OC:
            BIT_CLR(TCCR1A, u8_tCom1x0);
            BIT_SET(TCCR1A, u8_tCom1x1);
            break;
        case SET_OC:
            BIT_SET(TCCR1A, u8_tCom1x0);
            BIT_SET(TCCR1A, u8_tCom1x1);
            break;
        default:
            break;
    }
}

static void TIMER1_ConfigClock(const TIMER_CLOCK_t timerClock) {
    switch(timerClock) {
        case NO_CLOCK:
            BIT_CLR(TCCR1B, CS10);
            BIT_CLR(TCCR1B, CS11);
            BIT_CLR(TCCR1B, CS12);
            break;
        case F_CPU_CLOCK:
            BIT_SET(TCCR1B, CS10);
            BIT_CLR(TCCR1B, CS11);
            BIT_CLR(TCCR1B, CS12);
            break;
        case F_CPU_8:
            BIT_CLR(TCCR1B, CS10);
            BIT_SET(TCCR1B, CS11);
            BIT_CLR(TCCR1B, CS12);
            break;
        case F_CPU_64:
            BIT_SET(TCCR1B, CS10);
            BIT_SET(TCCR1B, CS11);
            BIT_CLR(TCCR1B, CS12);
            break;
        case F_CPU_256:
            BIT_CLR(TCCR1B, CS10);
            BIT_CLR(TCCR1B, CS11);
            BIT_SET(TCCR1B, CS12);
            break;
        case F_CPU_1024:
            BIT_SET(TCCR1B, CS10);
            BIT_CLR(TCCR1B, CS11);
            BIT_SET(TCCR1B, CS12);
            break;
        case F_CPU_EXT_CLK_FALLING:
            BIT_CLR(TCCR1B, CS10);
            BIT_SET(TCCR1B, CS11);
            BIT_SET(TCCR1B, CS12);
            break;
        case F_CPU_EXT_CLK_RISING:
            BIT_SET(TCCR1B, CS10);
            BIT_SET(TCCR1B, CS11);
            BIT_SET(TCCR1B, CS12);
            break;
        default:
            break;
    }
}

static void TIMER1_ConfigMode(const TIMER_MODE_t timerMode) {
    switch(timerMode) {
        case TIMER_MODE_NORMAL:
            BIT_CLR(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM_8:
            BIT_SET(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM_9:    
            BIT_CLR(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM_10:   
            BIT_SET(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_CTC_OCR:                
            BIT_CLR(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_FAST_PWM_8:             
            BIT_SET(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_FAST_PWM_9:             
            BIT_CLR(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_FAST_PWM_10:            
            BIT_SET(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_CLR(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_FREQ_CORRECT_ICR: 
            BIT_CLR(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_FREQ_CORRECT_OCR: 
            BIT_SET(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM_ICR:  
            BIT_CLR(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM_OCR:  
            BIT_SET(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_CLR(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_CTC_ICR:                
            BIT_CLR(TCCR1A, WGM10);
            BIT_CLR(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_FAST_PWM_ICR:           
            BIT_CLR(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        case TIMER_MODE_FAST_PWM_OCR:           
            BIT_SET(TCCR1A, WGM10);
            BIT_SET(TCCR1A, WGM11);
            BIT_SET(TCCR1B, WGM12);
            BIT_SET(TCCR1B, WGM13);
            break;
        default:
            break;
    }
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   PRIVATE FUNCTIONS OF TIMER2                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

static void TIMER2_ConfigClock(const TIMER_CLOCK_t clock) {
    switch(clock) {
        case NO_CLOCK:
            BIT_CLR(TCCR2, CS20);
            BIT_CLR(TCCR2, CS21);
            BIT_CLR(TCCR2, CS22);
            break;
        case F_CPU_CLOCK:
            BIT_SET(TCCR2, CS20);
            BIT_CLR(TCCR2, CS21);
            BIT_CLR(TCCR2, CS22);
            break;
        case F_CPU_8:
            BIT_CLR(TCCR2, CS20);
            BIT_SET(TCCR2, CS21);
            BIT_CLR(TCCR2, CS22);
            break;
        case F_CPU_32:
            BIT_SET(TCCR2, CS20);
            BIT_SET(TCCR2, CS21);
            BIT_CLR(TCCR2, CS22);
            break;
        case F_CPU_64:
            BIT_CLR(TCCR2, CS20);
            BIT_CLR(TCCR2, CS21);
            BIT_SET(TCCR2, CS22);
            break;
        case F_CPU_128:
            BIT_SET(TCCR2, CS20);
            BIT_CLR(TCCR2, CS21);
            BIT_SET(TCCR2, CS22);
            break;
        case F_CPU_256:
            BIT_CLR(TCCR2, CS20);
            BIT_SET(TCCR2, CS21);
            BIT_SET(TCCR2, CS22);
            break;
        case F_CPU_1024:
            BIT_SET(TCCR2, CS20);
            BIT_SET(TCCR2, CS21);
            BIT_SET(TCCR2, CS22);
            break;
        default:
            /* TODO: DEBUG    */
            break;
    }
}

static void TIMER2_ConfigMode(const TIMER_MODE_t timerMode) {
    switch(timerMode) {
        case TIMER_MODE_NORMAL:       
            BIT_CLR(TCCR2, WGM20);
            BIT_CLR(TCCR2, WGM21);
            BIT_SET(TCCR2, FOC2);   /*!< Set force output compare mode with non-PWM mode */
            break;
        case TIMER_MODE_CTC:         
            BIT_CLR(TCCR2, WGM20);
            BIT_SET(TCCR2, WGM21);
            BIT_SET(TCCR2, FOC2);   /*!< Set force output compare mode with non-PWM mode */
            break;
        case TIMER_MODE_FAST_PWM:   
            BIT_SET(TCCR2, WGM20);
            BIT_SET(TCCR2, WGM21);
            break;
        case TIMER_MODE_PHASE_CORRECT_PWM:
            BIT_SET(TCCR2, WGM20);
            BIT_CLR(TCCR2, WGM21);
            break;
        default:                
            /* TODO: DEBUG    */
            break;
    }
}

static void TIMER2_ConfigOC(const TIMER_MODE_t timerMode, const TIMER_OC_t compareMode) {
    switch(compareMode) {
        case NO_OC:
            BIT_CLR(TCCR2, COM20);
            BIT_CLR(TCCR2, COM21);
            break;
        case TOGGLE_OC:
            if((timerMode == TIMER_MODE_NORMAL) || (timerMode == TIMER_MODE_CTC)) {
                BIT_SET(TCCR2, COM20);
                BIT_CLR(TCCR2, COM21);
            }
            else{
                /* TODO: DEBUG    */
            }
            break;
        case CLEAR_OC:
            BIT_CLR(TCCR2, COM20);
            BIT_SET(TCCR2, COM21);
            break;
        case SET_OC:  
            BIT_SET(TCCR2, COM20);
            BIT_SET(TCCR2, COM21);
            break;
        default:
            /* TODO: DEBUG    */
            break;
    }
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              ISR FUNCTIONS                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* ISR of TIMER0 Overflow */
void __vector_11(void) __attribute__((signal));
void __vector_11(void) {
    GIE_Disable();

    TIMER0_OVF_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, TOV0);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER0 Compare Match */
void __vector_10(void) __attribute__((signal));
void __vector_10(void) {
    GIE_Disable();

    TIMER0_COMP_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, OCF0);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER1 Overflow */
void __vector_9(void) __attribute__((signal));
void __vector_9(void) {
    GIE_Disable();

    TIMER1_OVF_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, TOV1);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER1 Compare Match B */
void __vector_8(void) __attribute__((signal));
void __vector_8(void) {
    GIE_Disable();

    TIMER1_COMPB_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, OCF1B);   /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER1 Compare Match A */
void __vector_7(void) __attribute__((signal));
void __vector_7(void) {
    GIE_Disable();

    TIMER1_COMPA_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, OCF1A);   /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER1 Capture Event */
void __vector_6(void) __attribute__((signal));
void __vector_6(void) {
    GIE_Disable();

    TIMER1_CAPT_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, ICF1);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER2 Overflow */
void __vector_5(void) __attribute__((signal));
void __vector_5(void) {
    GIE_Disable();

    TIMER2_OVF_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, TOV2);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

/* ISR of TIMER2 Compare Match */
void __vector_4(void) __attribute__((signal));
void __vector_4(void) {
    GIE_Disable();

    TIMER2_COMP_CBK_PTR();

    BIT_SET(TIMER_u8_tTIFR_REG, OCF2);    /*!< Clear the interrupt flag */

    GIE_Enable();
}

