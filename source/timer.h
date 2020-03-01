/**
 * @file
 * @brief Timer module. 
 *
 */
#ifndef TIMER_H
#define TIMER_H

/**
 * @brief Set current time as start time.
 * 
 */
void timer_reset();


/**
 * @brief Get the time since start time.
 * 
 * @return Difference in seconds.
 * 
 */
int timer_get_time();

#endif