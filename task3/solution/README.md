
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

![Imaga small](https://github.com/BatyaPng/3_sem_22_23/blob/main/task3/solution/Pictures/small_file_size.png?raw=true)
<br/>
![Image medium](https://github.com/BatyaPng/3_sem_22_23/blob/main/task3/solution/Pictures/medium_file_size.png?raw=true)
<br/>
![Image large](https://github.com/BatyaPng/3_sem_22_23/blob/main/task3/solution/Pictures/big_file_size.png?raw=true)


### CONCLUSION:

The fastest IPC turned out to be __shared memory__. But this is not surprising, since this IPC simply creates a common area in RAM without any add-ons of other IPCs.
Next comes the __FIFO__ performance. It is also due to the simplicity of implementation.
__MSQ__ turned out to be the slowest.
