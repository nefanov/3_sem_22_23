#include "Integral.hpp"

size_t numUnder = 0;
pthread_mutex_t mutex;

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

void *CalcIntegrate(void *varss) {
    srand(time(NULL));

    fprintf(stderr, "I'm Here ))\n");

    data *vars = (data *) varss;

    interval range = vars->range;
    size_t numPoints = vars->numPoints;

    for (size_t i = numPoints; i > 0; i--) {
        point p = CreatePoint(range);
        // printf("x = %lg, y = %lg\n", p.x, p.y);

        if (IsUnder(p) == true) {
            pthread_mutex_lock(&mutex);
            
            numUnder++;

            pthread_mutex_unlock(&mutex);
        }        
    }

    printf("numunder in thread %ld\n", numUnder);

}