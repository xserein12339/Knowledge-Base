#include "servo.h"
#include "stdio.h"


int main() 
{
    servo_dev_t servo1 = NULL;
    timer_handle_t timer_handle = NULL;

    servo_create(&servo1);
    const servo_config_t cfg = {0};
    servo1->init(servo1, &cfg);
    servo1->set_target_angle(servo1, 99);
    printf("target angle = [%f]\r\n", servo1->target_angle);


    servo1->current_angle = 44;
    float current_angle;
    servo1->get_current_angle(servo1, &current_angle);
    printf("current angle = [%f]\r\n", current_angle);



    return 0;
}

