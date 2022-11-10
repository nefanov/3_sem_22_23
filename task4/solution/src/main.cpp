#include <assert.h>
#include "Integral.hpp"

extern size_t numUnder;

int main(int argc, char **argv) {
    assert(argc == 4);

    double leftBound = atof(argv[1]);
    double rightBound = atof(argv[2]);

    double length = rightBound - leftBound;
    
    int nThreads = atoi(argv[3]);

    double sizeSegment = length / nThreads;

    interval* ranges = (interval *) calloc(nThreads, sizeof(interval));
    ASSERTED(calloc, ranges, NULL, -1);

    ranges[0].left = leftBound;
    ranges[0].right = leftBound + sizeSegment;

    for (int i = 1; i < nThreads; i++) {
        ranges[i].left = ranges[i - 1].right; 
        ranges[i].right = ranges[i].left + sizeSegment;
    }

    data *tmpData = (data *) calloc(nThreads, sizeof(data));
    ASSERTED(calloc, tmpData, NULL, -1);

    for (int i = 0; i < nThreads; i++) {
        tmpData[i].numPoints = NUM_POINTS / nThreads;
        tmpData[i].range = ranges[i];
    }

    void *data = (void *) tmpData; 

    pthread_t *threads = (pthread_t *) calloc(nThreads, sizeof(pthread_t));
    ASSERTED(calloc, threads, NULL, -1);

    for (int i = 0; i < nThreads; i++) {
        pthread_create(&threads[i], NULL, CalcIntegrate, data + i);
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    double square = (rightBound - leftBound) * (CalcFuncValue(rightBound) - CalcFuncValue(leftBound));
    double integrateVal = ((double) numUnder / (double) NUM_POINTS) * square;

    printf("under points: %ld\n", numUnder);

    printf("Inegral x^2 from %lg to %lg is %lg\n", leftBound, rightBound, integrateVal);

    return 0;
}