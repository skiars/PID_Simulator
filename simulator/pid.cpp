#include "pid.h"

PID::PID(double kp, double ki, double kd)
{
    setParams(kp, ki, kd);
}

void PID::setParams(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void PID::setValue(double value)
{
    m_setValue = value;
}

double PID::iteration(double measured)
{
    double error = m_setValue - measured;
    double derror = error - m_error;

    m_sum += error;
    m_error = error;
    return m_kp * error + m_ki * m_sum + m_kd * derror;
}
