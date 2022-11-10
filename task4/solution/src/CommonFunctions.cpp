#include "CommonFunctions.hpp"

int CompDouble(const double a, const double b) {
    if (a - b > EPS)
        return MORE;
    else if (fabs(a - b) < EPS)
        return EQUAL;
    else
        return LESS;
}

double DRand(const double min, const double max) {

    double rndDouble = min + ((double) rand() / RAND_MAX) * (max - min);

    return rndDouble;
}