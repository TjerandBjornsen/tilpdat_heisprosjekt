/**
 * @file
 * @brief Functions used in main.c.
 * 
 */
#ifndef MAIN_H
#define MAIN_H





/**
 * @brief Initializes elevator; elevator drives down untill a floor sensor is activated.
 * 
 * @return STATE_IDLE.
 */ 



/**
 * @brief Polls buttons until order is placed or stop button pressed
 * 
 * @return STATE_SERVE if order in queue; STATE_STOPPED if stop button is pressed.
 */



/**
 * @brief Serves orders until queue empty or stop button pressed.
 * 
 * @return STATE_IDLE if queue empty; STATE_STOPPED if stop button is pressed.
 */



/**
 * @brief Sets motor direction towards target floor.
 * 
 */



/**
 * @brief 
 *
 */



/**
 * @brief Deals with stopping at ordered floors.
 * 
 */



/**
 * @brief Stops elevator, returns to idle if stop is not pressed.
 *
 * @return STATE_IDLE if stop button not pressed. 
 */



/**
 * @brief Polls floor sensors.
 * 
 * @return The floor number. -1 if not in floor.
 */



/**
 * @brief Polls order buttons and places orders in queue.
 * 
 */



/**
 * @brief Clears order lights for given floor.
 * 
 */



/**
 * @brief Clears all order lights.
 * 
 */



/**
 * @brief Enables interrupt for stopping elevator upon program exit.
 * 
 */


#endif