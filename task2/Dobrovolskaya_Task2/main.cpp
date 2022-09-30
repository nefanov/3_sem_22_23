#include "main.h"

int main()
{
    int result, letter;
    
    Pipe *pip = constructPipe();

    create_pipe(pip);

    result = fork();

    while (1)
    {
        letter = send(pip, result);
        if (letter == 0)
        {
            if (result == 0)
            {
                _exit(0);
            }
            break;
        }
        receive(pip, result);
    }

    destructPipe(pip);
   
    if (result == 0)
    {
        _exit(0);
    }

    return 0;
    
}

