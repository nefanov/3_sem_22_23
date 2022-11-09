
# HW 3
## IPC Analysis

### Tests were carried out with these parameters

| File size |        |      |
|-----------|--------|------|
| small     | medium | big  |
| 1 KB      | 1 MB   | 1 GB |

| Buffer size |        |      |
|-----------|--------|------|
| small     | medium | big  |
| 1 KB      | 1 MB   | 1 GB |


| Buff size |        |        |
|-----------|--------|--------|
| SMALL     | MEDIUM | BIG    |
| 8 B       | 1024 B | 4096 B |


### GRAPH: 

![Imaga small](/Pictures/small_file_size)
<br/>
![Image medium](https://github.com/shaazmik/3_sem_22_23/blob/main/task3/homework/img/medium.png?raw=true)
<br/>
![Image large](https://github.com/shaazmik/3_sem_22_23/blob/main/task3/homework/img/large.png?raw=true)


### CONCLUSION: 

The most effective in these tests was ___virtual-shared memory___, which allows you to immediately access data from another process,<br/>
without the preliminary cost of reading.<br/>
__Message queue__  is the most efficient for sending short messages.<br/>
__FIFO__ stably keeps in the middle among all and the easiest method to implement.<br/>
