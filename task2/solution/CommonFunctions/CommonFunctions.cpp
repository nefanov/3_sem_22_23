#include "CommonFunctions.hpp"

size_t GetFSize(FILE *fP) {
    fseek(fP, 0, SEEK_END);

    size_t fSize = (size_t) ftell(fP);

    fseek(fP, 0, SEEK_SET);

    return fSize;
}