/**************************************************************************
 * @file        TIMER_reg.h
 * @author      Mahmoud Karam (ma.karam272@gmail.com)
 * @brief       Registers of Timers of ATmega328p MCU
 * @version     1.0.0
 * @date        2021-07-31
 **************************************************************************/
#ifndef TIMER_REG_H
#define TIMER_REG_H

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*              CHANGE THIS PART ONLY FOR NEW DEVICES                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/**************************************************************************
 *                     Timer/Counter0 Register (8-bit)
 **************************************************************************/
#define TCCR0 (* ((volatile u8_t *) 0x53) )   /* Timer/Counter Control Register A */
#define TCNT0 (* ((volatile u8_t *) 0x52) )   /* Time Counter */
#define OCR0  (* ((volatile u8_t *) 0x51) )   /* Output Compare Register */

enum {
    CS00,       /* Clock Select Bit 0   */
    CS01,       /* Clock Select Bit 1   */
    CS02,       /* Clock Select Bit 2   */
    WGM01,      /* Waveform Generation Mode Bit 1   */
    COM00,      /* Compare Output Mode Bit 0        */
    COM01,      /* Compare Output Mode Bit 1        */
    WGM00,      /* Waveform Generation Mode Bit 0   */
    FOC0,       /* Force Output Compare Bit         */
};  /* TCCR0: Timer/Counter 0 Control Register */

/**************************************************************************
 *                     Timer/Counter1 Register (16-bit)
 **************************************************************************/
#define TCCR1A    (* ((volatile u8_t *) 0x4F) )
#define TCCR1B    (* ((volatile u8_t *) 0x4E) )
#define TCNT1H    (* ((volatile u8_t *) 0x4D) )
#define TCNT1L    (* ((volatile u8_t *) 0x4C) )
#define ICR1H     (* ((volatile u8_t *) 0x47) )
#define ICR1L     (* ((volatile u8_t *) 0x46) )
#define OCR1AH    (* ((volatile u8_t *) 0x4B) )
#define OCR1AL    (* ((volatile u8_t *) 0x4A) )
#define OCR1BH    (* ((volatile u8_t *) 0x49) )
#define OCR1BL    (* ((volatile u8_t *) 0x48) )

enum {
	WGM10,  /* Waveform Generation Mode 1 */
	WGM11,  /* Waveform Generation Mode 1 */
	FOC1B,  /* Force Output Compare for Compare unit B */
	FOC1A,  /* Force Output Compare for Compare unit A */
	COM1B0, /* Compare Output Mode 1 */
	COM1B1, /* Compare Output Mode 1 */
	COM1A0, /* Compare Output Mode 1 */
	COM1A1, /* Compare Output Mode 1 */
};  /* TCCR1A: Timer/Counter 1 Control Register A */

enum {
    CS10,  /* Clock Select Bit 0 */
    CS11,  /* Clock Select Bit 1 */
    CS12,  /* Clock Select Bit 2 */
    WGM12, /* Waveform Generation Mode Bit 2 */
    WGM13, /* Waveform Generation Mode Bit 3 */
    ICES1 = 6, /* Input Capture Edge Select */
    ICNC1, /* Input Capture Noise Canceler */
};  /* TCCR1B: Timer/Counter 1 Control Register B */

/**************************************************************************
 *                     Timer/Counter2 Register (8-bit)
 **************************************************************************/
#define TCCR2      (* ((volatile u8_t *) 0x45) )
#define TCNT2      (* ((volatile u8_t *) 0x44) )
#define OCR2       (* ((volatile u8_t *) 0x43) )

enum {
    CS20,    /* Clock Select Bit 0 */
    CS21,    /* Clock Select Bit 1 */
    CS22,    /* Clock Select Bit 2 */
    WGM21,   /* Waveform Generation Mode Bit 2 */
    COM20,   /* Compare Output Mode Bit 0 */
    COM21,   /* Compare Output Mode Bit 1 */
    WGM20,   /* Waveform Generation Mode Bit 0 */
    FOC2,    /* Force Output Compare Bit */
};  /* TCCR2: Timer/Counter 2 Control Register */

/**************************************************************************
 *                     Timers/Counters Common Registers
 **************************************************************************/
#define TIMER_u8_tTIMSK_REG     (* ((volatile u8_t *) 0x59) )
#define TIMER_u8_tTIFR_REG      (* ((volatile u8_t *) 0x58) )

enum {
    TOIE0,  /* Timer/Counter0 Overflow Interrupt Enable */
    OCIE0,  /* Timer/Counter0 Output Compare Match Interrupt Enable */
    TOIE1,  /* Timer/Counter1 Overflow Interrupt Enable */
    OCIE1B, /* Timer/Counter1 Output Compare Match B Interrupt Enable */
    OCIE1A, /* Timer/Counter1 Output Compare Match A Interrupt Enable */
    TICIE1, /* Timer/Counter1 Input Capture Interrupt Enable */
    TOIE2,  /* Timer/Counter2 Overflow Interrupt Enable */
    OCIE2,  /* Timer/Counter2 Output Compare Match Interrupt Enable */

};  /* TIMSK: Timer/Counter Interrupt Mask Register */

enum {
    TOV0,  /* Timer/Counter0 Overflow Flag */
    OCF0,  /* Timer/Counter0 Output Compare Match Flag */
    TOV1,  /* Timer/Counter1 Overflow Flag */
    OCF1B, /* Timer/Counter1 Output Compare Match B Flag */
    OCF1A, /* Timer/Counter1 Output Compare Match A Flag */
    ICF1,  /* Timer/Counter1 Input Capture Flag */
    TOV2,  /* Timer/Counter2 Overflow Flag */
    OCF2,  /* Timer/Counter2 Output Compare Match Flag */
};  /* TIFR: Timer/Counter Interrupt Flag Register */

#endif    /* TIMER_REG_H */