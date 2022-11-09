#include "Integral.hpp"

int main() {
    interval range = {2, 9};

    double res = CalcIntegrate(range, NUM_POINTS);

    printf("Inegral x^2 from %lg to %lg is %lg\n", range.left, range.right, res);

    return 0;
}