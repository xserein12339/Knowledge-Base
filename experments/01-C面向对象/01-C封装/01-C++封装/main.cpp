#include <iostream>
#include "Servo.hpp"

using namespace std;

int main(int argc, char **argv)
{
    void *test_handle;
    Servo servo1;
    servo1.servo_init(&test_handle, 1, 1);
    servo1.servo_set_param(0.5, 2.5, 0, 180);
    servo1.servo_set_contral(100, 200, 1);
    servo1.servo_set_target_angle(50);
    float current_angle;
    servo1.servo_get_current_angle(&current_angle);
    cout << current_angle << endl;
    return 0;
}