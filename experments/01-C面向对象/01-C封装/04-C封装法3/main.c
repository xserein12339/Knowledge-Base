#include "servo.h"


int main()
{
    servo_handle_t servo1 = NULL;
    esp32_servo_create(&servo1);
    servo_config_t cfg = {
        .name = "servo1",
    };
    servo_init(servo1, &cfg);
    servo_set_target_angle(servo1, 100);
    float current_angle;
    servo_get_current_angle(servo1, &current_angle);

    esp32_servo_delete(&servo1);
    return 0;
}


