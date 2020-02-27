/**
 * @file
 * @brief Functions used in main.c.
 * 
 */
#ifndef MAIN_H
#define MAIN_H

typedef enum {
    STATE_UNDEFINED,
    STATE_IDLE,
    STATE_SERVING,
    STATE_STOPPED
} State;

/**
 * @brief Initializes elevator; elevator drives down untill a floor sensor is activated.
 * 
 * @return STATE_IDLE.
 */ 
static State elevator_init();

/**
 * @brief Polls buttons until order is placed or stop button pressed
 * 
 * @return STATE_SERVE if order in queue; STATE_STOPPED if stop button is pressed.
 */
static State elevator_idle();

/**
 * @brief Serves orders until queue empty or stop button pressed.
 * 
 * @return STATE_IDLE if queue empty; STATE_STOPPED if stop button is pressed.
 */
static State elevator_serve();

/**
 * @brief Sets motor direction towards target floor.
 * 
 */
static void elevator_set_motor_direction(int* started_between_floors, int first_order_served);

static void elevator_floor_arrival(int floor, int* first_order_served, State* state);

/**
 * @brief Sub state of STATE_SERVING. Deals with stopping at ordered floors.
 * 
 */
static State elevator_floor_stop(int floor);

/**
 * @brief Stops elevator, returns to idle if stop is not pressed.
 *
 * @return STATE_IDLE if stop button not pressed. 
 */
static State elevator_stop();


/**
 * @brief Polls floor sensors.
 * 
 * @return The floor number. -1 if not in floor.
 */
static int poll_floor_sensors();


/**
 * @brief Polls order buttons and places orders in queue.
 * 
 */
static void update_queue();


/**
 * @brief Enables interrupt for stopping elevator upon program exit.
 * 
 */
static void sigint_handler(int sig);


static void clear_order_lights(int floor);


/**
 * @brief Clears all order lights.
 * 
 */
static void clear_all_order_lights();

#endif