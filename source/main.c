#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "main.h"
#include "hardware.h"
#include "queue.h"
#include "timer.h"


static int m_prev_floor = -1;
static int m_above_prev_floor = -1;
static int m_departed_from_floor = -1;
static int m_current_floor = -1;
static int m_next_queue_floor = -1;
static int m_motor_dir = -1;


int main(){
    State state = STATE_UNDEFINED;
    State (*fun_ptr)();

    while (1) {
        switch (state) {
            case STATE_UNDEFINED:
                fun_ptr = &elevator_init;
                break;

            case STATE_IDLE:
                fun_ptr = &elevator_idle;
                break;
            
            case STATE_SERVING:
                fun_ptr = &elevator_serve;
                break;

            case STATE_STOPPED:
                fun_ptr = &elevator_stop;
                break;
            
            default:
                printf("ERROR\n");
                return 1;
        }
        state = (*fun_ptr)();
    }
    return 0;
}


static State elevator_init() {
    printf("Initializing...\n");

    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);                              
    
    int motor_activated = 0;
    m_motor_dir = 2;

    while (1) {
        m_current_floor = poll_floor_sensors();      

        // If in a floor -> Initialized                    
        if (m_current_floor != -1) {                                                
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);           
            hardware_command_floor_indicator_on(m_current_floor);               
            m_prev_floor = m_current_floor;
            m_above_prev_floor = 0;
            m_departed_from_floor = 0;               
            printf("Initialized\n");
            return STATE_IDLE;                                                 
        }

        // Activate motor only once
        if (!motor_activated) {                                      
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);        
            motor_activated = 1;
        }
    }
}


static State elevator_idle() {
    while (1) {
        if (hardware_read_stop_signal())                                
            return STATE_STOPPED;

        update_queue();                                     
        m_next_queue_floor = queue_read(m_prev_floor, m_motor_dir); 
        if (m_next_queue_floor != -1)                                   
            return STATE_SERVING;
    }
}


static State elevator_serve() {
    m_current_floor = poll_floor_sensors();
    
    // Check if started serving between floors
    int started_serving_between_floors = 0;
    int first_order_served = 1;
    if (m_current_floor == -1) {
        started_serving_between_floors = 1;
        first_order_served = 0;
    }
    
    while (1) {
        if (hardware_read_stop_signal())                            
            return STATE_STOPPED;

        update_queue();
        m_next_queue_floor = queue_read(m_prev_floor, m_motor_dir);
    
        if (m_next_queue_floor == -1)                                 
            return STATE_IDLE;

        m_current_floor = poll_floor_sensors();
        elevator_set_motor_direction(&started_serving_between_floors, first_order_served);

        // Check if arrived at floor
        for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; ++f) {
            if (f == m_current_floor) {
                State state;
                elevator_floor_arrival(f, &first_order_served, &state);
                if (state == STATE_STOPPED) 
                    return state;
            }
        }
    }
}


static void elevator_set_motor_direction(int* started_serving_between_floors, int first_order_served) {
    if (m_next_queue_floor == m_prev_floor) {
        // Set motor direction towards previous floor
        if (*started_serving_between_floors) {
            switch (m_above_prev_floor) {
            case 0:
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                m_motor_dir = 2;
                break;
                
            case 1:
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                m_motor_dir = 0;
                break;
            }
            *started_serving_between_floors = 0;
        }

        // Clear order to current floor if started serving between floors scenario is already dealt with
        else if (first_order_served) {
            queue_clear(m_current_floor);
            clear_order_lights(m_current_floor);
            m_next_queue_floor = queue_read(m_prev_floor, m_motor_dir);
        }
    }
    else if (m_next_queue_floor > m_prev_floor) {
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        m_motor_dir = 0;
        if (!m_departed_from_floor) {
            m_above_prev_floor = 1;
            m_departed_from_floor = 1;
        }
    }
    else {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        m_motor_dir = 2;
        if (!m_departed_from_floor) {
            m_above_prev_floor = 0;
            m_departed_from_floor = 1;
        }
    }
}


static void elevator_floor_arrival(int floor, int* first_order_served, State* state) {
    hardware_command_floor_indicator_on(floor);
    m_prev_floor = floor;

    if (m_motor_dir == 0)           
        m_above_prev_floor = 1;
    else                            
        m_above_prev_floor = 0;

    if (m_next_queue_floor == floor) {
        m_above_prev_floor = 0;
        *first_order_served = 1;
        elevator_floor_stop(floor, state);
        m_departed_from_floor = 0;
    }
}


static void elevator_floor_stop(int floor, State* state) {
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_door_open(1);
    
    queue_clear(floor);
    clear_order_lights(floor);

    timer_reset();

    while (1) {
        if (hardware_read_stop_signal()) {
            *state = STATE_STOPPED;
            return;
        }

        if (timer_get_time() >= 3) {
            hardware_command_door_open(0);
            return;
        }

        if (hardware_read_obstruction_signal()) 
            timer_reset();

        update_queue();
        queue_clear(floor);
        clear_order_lights(floor);
    }
}


static State elevator_stop() {
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);
    
    queue_clear_all();
    clear_all_order_lights();

    if (m_current_floor != -1)
        hardware_command_door_open(1);

    int first_order_registered = 0;

    while (1) {
        if (hardware_read_stop_signal()) {
            hardware_command_stop_light(1);
            queue_clear_all();
            clear_all_order_lights();
            first_order_registered = 0;
            timer_reset();
        }
        else {
            hardware_command_stop_light(0);

            if (m_current_floor == -1) 
                return STATE_IDLE;

            if (hardware_read_obstruction_signal())
                timer_reset();

            if (timer_get_time() >= 3) {
                hardware_command_door_open(0);
                return STATE_IDLE;
            }

            update_queue();
            queue_clear(m_current_floor);
            clear_order_lights(m_current_floor);
            m_next_queue_floor = queue_read(m_prev_floor, m_motor_dir);

            // Set motor direction towards first ordered floor
            if (!first_order_registered && m_next_queue_floor != -1) {
                if (m_next_queue_floor > m_current_floor) 
                    m_motor_dir = 0;
                else 
                    m_motor_dir = 2;
                first_order_registered = 1;
            }
        }
    }
} 


static int poll_floor_sensors() {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i) {
        if (hardware_read_floor_sensor(i))
            return i;
    }
    return -1;
}


static void update_queue() {
    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; ++f) {
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
            queue_place_order(f, QUEUE_ORDER_INSIDE, m_prev_floor, m_above_prev_floor);
            hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
        }
        if(hardware_read_order(f, HARDWARE_ORDER_UP)){
            queue_place_order(f, QUEUE_ORDER_UP, m_prev_floor, m_above_prev_floor);
            hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
        }
        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
            queue_place_order(f, QUEUE_ORDER_DOWN, m_prev_floor, m_above_prev_floor);
            hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
        }
    }
}


static void clear_order_lights(int floor) {
    hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
}


static void clear_all_order_lights(){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        clear_order_lights(f);
    }
}


static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}
