/**
 * @file
 * @brief Queue module.
 *
 * Processes floor orders and specifies an elevators next target floor.
 *
 */
#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_NUMBER_OF_FLOORS 4

/**
 * @brief Order type used in @c queue_place_order.
 */
typedef enum {
    QUEUE_ORDER_UP,
    QUEUE_ORDER_INSIDE,
    QUEUE_ORDER_DOWN
} QueueOrder;


/**
 * @brief Places an order in queue.
 * 
 * @param to_floor The floor which the elevator is ordered to. Valid floors: 0-3.
 * @param order_type Type of order. Either an order made from inside, an order made from outside wanting to go 
 * go down, or an order made from outside wanting to go up.
 * @param prev_floor The previous floor which the elevator was in. Valid floors: 0-3.
 * @param above Non-zero value to specify that the elevator is above the previous floor, 0 to specify 
 * in or below previous floor.
 */
void queue_place_order(int to_floor, QueueOrder order_type, int prev_floor, int above);


/**
 * @brief Reads the next target floor, depending on the elevators location and motor direction.
 *
 * @param prev_floor The previous floor which the elevator was in. Valid floors: 0-3.
 * @param motor_dir The elevators current motor direction. 0 to specify up, 2 to specify down.
 * 
 * @return The elevators next target floor; next floor to stop at.
 */
int queue_read(int prev_floor, int motor_dir);


/**
 * @brief Clears all entries in queue for a given floor.
 * 
 * @param floor The floor to clear from queue. Valid floors: 0-3.
 */
void queue_clear(int floor);

/**
 * @brief Clears all entries in queue for all floors.
 */
void queue_clear_all();

#endif
