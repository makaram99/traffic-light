/**************************************************************************
 * @file        EXTI_reg.h
 * @author      Mahmoud Karam (ma.karam272@gmail.com)
 * @brief       EXTI Registers of ATmega328p MCU
 * @version     1.0.0
 * @date        2021-07-31
 * @copyright   Copyright (c) 2022
 **************************************************************************/
#ifndef EXTI_REG_H
#define EXTI_REG_H


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*              CHANGE THIS PART ONLY FOR NEW DEVICES                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#define MCUCR      (* ((volatile u8_t *) 0x55) )    /* MCU Control Register */
#define MCUCSR     (* ((volatile u8_t *) 0x54) )    /* MCU Control and Status Register */
#define GICR	   (* ((volatile u8_t *) 0x5B) )    /* General Interrupt Control Register */
#define GIFR       (* ((volatile u8_t *) 0x5A) )    /* General Interrupt Flag Register */

enum {
	ISC00,                                          /* Interrupt Sense Control 0 Bit 0 */
	ISC01,                                          /* Interrupt Sense Control 0 Bit 1 */
	ISC10,                                          /* Interrupt Sense Control 1 Bit 0 */
	ISC11,	                                        /* Interrupt Sense Control 1 Bit 1 */
};	/* MCUCR	*/

enum {
	ISC2 = 6,	                                    /* Interrupt Sense Control 2 */
};	/* MCUCSR	*/

enum {
	INT2 = 5,                                       /* External Interrupt Request 2 Enable */
	INT0,                                           /* External Interrupt Request 0 Enable */
	INT1,                                           /* External Interrupt Request 1 Enable */
};	/* GICR		*/

enum {
	INTF2 = 5,                                      /* External Interrupt Flag 2 */
	INTF0,                                          /* External Interrupt Flag 0 */
	INTF1,	                                        /* External Interrupt Flag 1 */
};	/* GIFR		*/

#endif    /* EXTI_REG_H */