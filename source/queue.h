/**
 * @file
 * @brief Queue module.
 *
 * Processes floor orders and
 * sorts them into a que.
 *
 */
#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_NUMBER_OF_FLOORS 4

typedef enum {
    QUEUE_ORDER_UP,
    QUEUE_ORDER_INSIDE,
    QUEUE_ORDER_DOWN
} QueueOrder;


/**
 * @brief Places order in queue.
 *
 */
void queue_place_order(int to_floor, QueueOrder order_type, int prev_floor, int above);


/**
 * @brief Reads next floor from queue. Must be polled.
 *
 * @return The first floor number in queue.
 */

int queue_read(int prev_floor, int motor_dir);


/**
 * @brief Clears queue at floor.
 * 
 */
void queue_clear(int floor);

/**
 * @brief Clears queue at all floors.
 * 
 */
void queue_clear_all();

#endif
