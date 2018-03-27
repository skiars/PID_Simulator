//=============================================================================
// file name : pid.c
// author    : Wenliang Guan
// version   : 1.0
// date      : 2018/3/27
// This is the implementation of a classic PID controller.
//=============================================================================

#include "pid.h"

// PID initialization
void pid_init(pid_control_t *pid, float kp, float ki, float kd, int scale)
{
    pid->kp = (int)(kp * scale);
    pid->ki = (int)(ki * scale);
    pid->kd = (int)(kd * scale);
    pid->scale = scale;
    pid->sum = 0;
    pid->error = 0;
    pid->value = 0;
}

// PID setting the set value
void pid_set_value(pid_control_t* pid, int value)
{
    pid->value = value;
}

// PID setting parameters (floating-point)
void pid_set_params(pid_control_t* pid, float kp, float ki, float kd)
{
    pid->kp = (int)(kp * pid->scale);
    pid->ki = (int)(ki * pid->scale);
    pid->kd = (int)(kd * pid->scale);
}

// PID iteration calculation
int pid_iteration(pid_control_t* pid, int measured)
{
    int error, derror;

    error = pid->value - measured;  // calculation of the current error
    derror = error - pid->error;    // calculation error derivative
    pid->sum += error;              // calculation error integral
    pid->error = error;             // update error value

    /* calculation PID controller output value */
    return (pid->kp * error         // proportion control
          + pid->ki * pid->sum      // integral control
          + pid->kd * derror)       // derivative control
          / pid->scale;             // integral point scaling
}
