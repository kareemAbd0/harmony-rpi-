#include <iostream>
#include <cmath>
#include "control.h"

double Control::getControl(double xl, double yl, double xf, double yf, double vl, double vf) {
    double kp = 1;
    double c = -2.0;
    double d = std::sqrt(std::pow(xf - xl, 2) + std::pow(yf - yl, 2));
    std::cout << "xl: " << xl << " yl: " << yl << " xf: " << xf << " yf: " << yf << " vl: " << vl << " vf: " << vf << std::endl;
    std::cout << "distance: " << d << std::endl;
    double a = kp * (d + c);
    std::cout << "acceleration: "<< a << std::endl;
    return fmin(1,fmax(0,(a)));
}
