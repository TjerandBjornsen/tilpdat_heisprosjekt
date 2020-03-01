#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

static int m_up_queue[4] = {0, 0, 0, 0};
static int m_down_queue[4] = {0, 0, 0, 0};


void queue_place_order(int ordered_floor, QueueOrder order_type, int prev_floor, int above) {
    if (ordered_floor == QUEUE_NUMBER_OF_FLOORS - 1) 
        m_up_queue[ordered_floor] = 1;

    else if (ordered_floor == 0) 
        m_down_queue[ordered_floor] = 1;

    else if (order_type == QUEUE_ORDER_UP) 
        m_up_queue[ordered_floor] = 1;

    else if (order_type == QUEUE_ORDER_DOWN) 
        m_down_queue[ordered_floor] = 1;

    else if (order_type == QUEUE_ORDER_INSIDE) {
        if (ordered_floor == prev_floor) {
            if (above) 
                m_down_queue[ordered_floor] = 1;
            else 
                m_up_queue[ordered_floor] = 1;
        }

        else if (ordered_floor > prev_floor)
            m_up_queue[ordered_floor] = 1;

        else if (ordered_floor < prev_floor) 
            m_down_queue[ordered_floor] = 1;
    }
}


int queue_read(int prev_floor, int motor_dir) {
    switch (motor_dir){
    case 0:
        for (int i = prev_floor + 1; i < QUEUE_NUMBER_OF_FLOORS; ++i){ 
            if (m_up_queue[i] == 1)
                return i;
        }
        for (int i = QUEUE_NUMBER_OF_FLOORS - 1; i > -1; --i){
            if (m_down_queue[i] == 1)
                return i;
        }
        for (int i = 0; i <= prev_floor; ++i) {
            if (m_up_queue[i] == 1) 
                return i;
        } 
        break;

    case 2:         
        for (int i = prev_floor - 1; i > -1; --i){ 
            if (m_down_queue[i] == 1)
                return i;
        }
        for (int i = 0; i < QUEUE_NUMBER_OF_FLOORS; ++i){ 
            if (m_up_queue[i] == 1)
                return i;
        }
        for (int i = QUEUE_NUMBER_OF_FLOORS - 1; i >= prev_floor; --i) {
            if (m_down_queue[i] == 1) 
                return i;
        }
        break;
    }
    return -1;
}


void queue_clear(int floor) {
    m_up_queue[floor] = 0;
    m_down_queue[floor] = 0;
}


void queue_clear_all() {
    for (int f = 0; f < QUEUE_NUMBER_OF_FLOORS; ++f) {
        queue_clear(f);
    }
}