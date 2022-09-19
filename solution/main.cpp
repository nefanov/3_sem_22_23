#include "io.hpp"
#include "solve.hpp"

int main() {
    char *str = NULL;

    input(&str);
    
    command_line cmd = parse(str);

    printf("%s\n", cmd.prss[0].name);
    printf("%s", cmd.prss[1].name);
}