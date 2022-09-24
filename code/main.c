/***************************************************************************
 * @file 	main.c
 * @author 	Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief 	main file. It is a generic file independent of the application.
 * @version 1.0.0
 * @date 	23 Sep 2022
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

