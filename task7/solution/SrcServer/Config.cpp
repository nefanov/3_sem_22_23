#include "Config.hpp"

int skip_spaces(char *str){
    int i = 0;
    while( str[i] != '\0' && isspace(str[i])) i++;

    return i;
}