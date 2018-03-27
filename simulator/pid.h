#ifndef PID_H
#define PID_H

class PID
{
public:
    PID(double kp = 0, double ki = 0, double kd = 0);
    void setParams(double kp, double ki, double kd);
    void setValue(double value);
    double iteration(double measured);

private:
    double m_kp, m_ki, m_kd;
    double m_setValue = 0, m_error = 0, m_sum = 0;
};

#endif // PID_H
