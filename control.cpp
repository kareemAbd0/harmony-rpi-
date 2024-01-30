#include "control.h"

double Control::getControl(double xl, double yl, double xf, double yf, double vl, double vf) {
    double kp = 0.8;
    double c = -0.55;
    return 0.5 - kp * (std::sqrt(std::pow(xf - xl, 2) + std::pow(yf - yl, 2)) + c);
}
