#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

int up_queue[4] = {0, 0, 0, 0};
int down_queue[4] = {0, 0, 0, 0};

void queue_place_order(int to_floor, QueueOrder order_type, int prev_floor, int motor_dir) {


    switch (motor_dir) {
    case 0:             // Elevator going up
        if (to_floor == QUEUE_NUMBER_OF_FLOORS - 1) {    // Last floor independent of direction
            up_queue[to_floor] = 1;
        }

        else if (to_floor > prev_floor && !(order_type == QUEUE_ORDER_DOWN)) {  // Floor on way up, not wanting to go down

            up_queue[to_floor] = 1;
        }
        else {
            down_queue[to_floor] = 1;                 // If order not on way up, place in queue on way down
        }
        break;

    case 2:             // Elevator going down
        if (to_floor == 0) {    // First floor independent of direction
            down_queue[to_floor] = 1;
        }

        else if (to_floor < prev_floor && !(order_type == QUEUE_ORDER_UP)) {  // Floor on way down, not wanting to go up

            down_queue[to_floor] = 1;
        }
        else {
            up_queue[to_floor] = 1;               // If order not on way down, place in queue on way up
        }
        break;

    default:
        break;
    }
}


void queue_print() {

    printf("Up_queue:\n");
    for (int i = 0; i < 4; ++i) {
        printf("%d, ", up_queue[i]);
    }
    printf("\n");
    printf("Down_queue:\n");
    for (int i = 0; i < 4; ++i) {
        printf("%d, ", down_queue[i]);
    }
    printf("\n\n");
}


int queue_read(int prev_floor, int motor_dir) {
    


    switch (motor_dir){
        case 0:         //Elevator going up
            for (int i = prev_floor; i < QUEUE_NUMBER_OF_FLOORS; ++i){ //Returnerer den nærmeste ovenfor i up_queue
                if (up_queue[i] == 1){
                    return i;
                }
            }

            for (int i = QUEUE_NUMBER_OF_FLOORS - 1; i > -1; --i){ //Returnerer den øverste i down_queue
                if (down_queue[i] == 1){
                    return i;
                }
            } //Tanke: Motorretning må settes etter at pop_queue har returnert, f.eks ved at returverdien sjekkes mot prev_floor

            break;

        case 2:         //Elevator going down
            for (int i = prev_floor; i > -1; --i){ //Returnerer den nærmeste nedenfor i down_queue
                if (down_queue[i] == 1){
                    return i;
                }
            }

            for (int i = 0; i < QUEUE_NUMBER_OF_FLOORS; ++i){ //Returnerer den laveste i up_queue
                if (up_queue[i] == 1){
                    return i;
                }
            }

            break;

    }

    return -1; //Hvis target_floor = -1 ennå, kan dette f.eks bety at begge køene er tomme.
}


void queue_clear(int floor) {
    up_queue[floor] = 0;
    down_queue[floor] = 0;
}


void queue_clear_all() {
    for (int f = 0; f < QUEUE_NUMBER_OF_FLOORS; ++f) {
        queue_clear(f);
    }
}