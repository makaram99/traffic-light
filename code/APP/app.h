/*********************************************************************************
 * @file 	app.h
 * @author 	Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief 	APIs of the application.
 * @version 1.0.0
 * @date 	23 Sep 2022
 * @copyright Mahmoud Karam Emara 2022, MIT License
 ********************************************************************************/


#ifndef APP_H_
#define APP_H_

/*********************************************************************************
 * @brief   Initialize the application
 * @details Initialize the application by initializing the peripherals and
 *          setting the initial state
 * @param   void
 * @return  void
 ********************************************************************************/
void APP_Init(void);

/*********************************************************************************
 * @brief   Start the application
 * @details Start the application by updating the state
 * @param   void
 * @return  void
 ********************************************************************************/
void APP_Start(void);


#endif /* APP_H_ */