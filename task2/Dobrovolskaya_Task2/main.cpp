#include "main.h"

int main()
{
    int result, letter;
    
    Pipe *pip = constructPipe();

    create_pipe(pip);

    result = fork();

    while (1)
    {
        letter = send(pip, result);//родитель отправляет ребенку файл text.txt
        if (letter == 0)
        {
            if (result == 0)
            {
                _exit(0);
            }
            break;
        }
        receive(pip, result);//ребенок возращает его родителю, родитель печатает его в text2.txt
    }

    destructPipe(pip);
   
    if (result == 0)
    {
        _exit(0);
    }

    return 0;
    
}

