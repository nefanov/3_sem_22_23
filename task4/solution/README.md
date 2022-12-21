# Monte Carlo integral counter
This program compute integral of `f(x) = exp(x)` from 0 to 20 for x. We count time for cases with different number of threads.
## Execution
To execute programm type `python3 wrapper.py`, then follow instruction of python input.  
Programm requires: `python 3.x`, `matplotlib + numpy libs`, `gcc compiler`.  
## Particular qualities
Programm use urandom programm to compute more random points pattern, if you do not need this remove `#define TRUE_RANDOM` in `integrator.c`.  
Programm can truly compute integral split rectangle for cells, but for test we split region for vertical parts. (it is hard to make cellular space using only count of cells, but this does not affect on result).  
## Examples and results
Runned test started with one thread and up to 36 threads.  
Machine properties: AMD Ryzen 5 4600H with Radeon Graphics (6 physycal cores, 12 threads)  
Results of time measuring (time measured with standart `time.h` functions in main thread):  
![Time measuring for f(x) = exp(x) in x = [0, 20]](result.png "Time measuring for f(x) = exp(x) in x = [0, 20]")