#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
 
double func(double x)
{
        return x;
}
 
int main()
{
        int a = 0.0;
        int b = 1.0;
        int points = 10000; 
        double s=0; 
        double x;
        double y;
        
        srand((unsigned)time(NULL));
        for (int i = 0; i < points; i++) 
        {
             x = (double)rand()/RAND_MAX; //генерируем x в интервале [0,1]
             y = (double)rand()/RAND_MAX; //генерируем y в интервале [0,1]
             if(fabs((double)y)-func(x) < 0) s += 1;
	     printf("%lf %lf %lf\n", s, x, y);
        }

        s=(s/(double)points) * (b-a) * (b-a);
        printf("\nResult= %lf\n", s);
        return 0;
}
