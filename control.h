#ifndef CONTROL_H
#define CONTROL_H

#include <cmath>

class Control {
public:
    static double getControl(double xl, double yl, double xf, double yf, double vl, double vf);
};

#endif // CONTROL_H

