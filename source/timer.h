/**
 * @file
 * @brief Timer module. Stores a start time and returns the difference of start time and current time.
 *
 */
#ifndef TIMER_H
#define TIMER_H

/**
 * @brief Resets timer.
 * 
 */
void timer_reset();


/**
 * @brief Gets the time since last reset.
 * 
 * @return Difference in seconds.
 * 
 */
int timer_get_time();

#endif