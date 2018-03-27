#ifndef TRANSFUNC_H
#define TRANSFUNC_H


class TransFunc
{
public:
    TransFunc();
    double drivingFunction(double t);
    double iteration(double x);

private:
    double z[2];
};

#endif // TRANSFUNC_H
