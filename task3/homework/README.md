
# HW 3
## Interprocessor Communication Analysis

### LAUNCH
__Calling the help line__ <br/>
```bash
  python3 start.py -help
```
__Start tests__ <br/>
``` bash
  python3 start.py -flag
```
___FLAGS:___ ```-small```, ```-medium```, ```-large```<br/>

### Tests were carried out with these parameters

|FLAG  | -small |-medium|-large|
|------|--------|-------|------|
|INPUT | 4kB    | 104Mb |2.1Gb |


|BUF IN BYTES  |512 | 4096 | 65536 | 
|--------------|----|------|-------|


### GRAPH: 



### CONCLUSION: 

The most effective in these tests was ___virtual-shared memory___, which allows you to immediately access data from another process,<br/>
without the preliminary cost of reading.<br/>
__Message queue__  is the most efficient for sending short messages.<br/>
__FIFO__ stably keeps in the middle among all and the easiest method to implement.<br/>
