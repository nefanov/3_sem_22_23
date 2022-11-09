#include "Integral.hpp"

double CalcFuncValue(const double x) {
    return x * x;
}

point CreatePoint(const interval range) {
    double xMin = range.left;
    double xMax = range.right + EPS;
    double yMax = CalcFuncValue(range.right) + EPS;

    point p = {};

    p.x = DRand(xMin, xMax);
    p.y = DRand(0, yMax);

    return p;
}

bool IsUnder(const point p) {
    if (CompDouble(p.y, CalcFuncValue(p.x)) == LESS)
        return true;
    return false;
}

double CalcIntegrate(const interval range, const size_t numPoints) {
    size_t numUnder = 0;

    for (size_t i = numPoints; i > 0; i--) {
        point p = CreatePoint(range);
        if (IsUnder(p) == true)
            numUnder++;
    }

    double square = (range.right - range.left) * (CalcFuncValue(range.right) - CalcFuncValue(range.left));
    double integrateVal = ((double) numUnder / (double) numPoints) * square;

    return integrateVal;
}