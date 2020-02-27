#include <time.h>
#include "timer.h"

int start_time = 0;

void timer_reset() {
    start_time = time(NULL);
}


int timer_get_time() {
    return (int)(time(NULL) - start_time);
}