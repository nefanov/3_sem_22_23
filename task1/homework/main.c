#include "pcmd.h"
#include "tokenizator.h"

int main()
{
    fprintf(stdout, "BASH:(0)_(0): ");

    struct CmdTokens s_cmd = {};

    if (constructTokens(&s_cmd) == CONSTRUCT_ERR)
    {
        fprintf(stderr, "ERROR: Unable to construct token structure\n");
        destructEmptyTokens(&s_cmd);
        return CONSTRUCT_ERR;
    }

    if (tokenizator(&s_cmd) == MAX_NUM_TOKENS_ERR)
    {
        fprintf(stderr, "ERROR: Maximum number of commands exceeded\n");
        destructEmptyTokens(&s_cmd);
        return MAX_NUM_TOKENS_ERR;
    }

    run(&s_cmd);

    destructTokens(&s_cmd);

    return 0;
}


