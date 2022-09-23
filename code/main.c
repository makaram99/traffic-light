/***************************************************************************
 * @file 	main.c
 * @author 	Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief 	Lab3: LM35 - Embedded Systems Foundation Diploma
 * @details Write a program the indicate the temperature using LCD and 3 LEDs:
 *          * If the reading (0> x < 50) 1 LED is turned on, 
 *          * If the reading (50> x < 100) 2 LEDs are turned on, 
 *          * If the reading (x > 100) 3 LEDs are turned on.
 * @version 1.0.0
 * @date 	2022-07-02
 * @copyright Mahmoud Karam Emara 2022, MIT License
 ***************************************************************************/

#include "APP/app.h"

int main (void){    
    APP_Init();

    while(1) {
        APP_Start();
	}

	return 0;
}

