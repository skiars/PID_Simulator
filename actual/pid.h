//=============================================================================
// file name : pid.h
// author    : Wenliang Guan
// version   : 1.0
// date      : 2018/3/27
// This is the implementation of a classic PID controller.
//=============================================================================

#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
#if __cplusplus
  extern "C" {
#endif
#endif // __cplusplus

typedef struct {
    int kp, ki, kd;     // PID parameter
    int value;          // set value
    int error;          // last error
    int sum;            // error integral
    int scale;          // integral point scaling factor
} pid_control_t;

void pid_init(pid_control_t* pid, float kp, float ki, float kd, int scale);
void pid_set_value(pid_control_t *pid, int value);
void pid_set_params(pid_control_t* pid, float kp, float ki, float kd);
int pid_iteration(pid_control_t* pid, int measured);

#ifdef __cplusplus
#if __cplusplus
  }
#endif
#endif // __cplusplus

#endif
