
# Home Assigmnet 3 | IPC ANALYSIS

### TEST SUBJECTS

|IPC | SHARED MEM | MES Q | FIFO |
|----|------------|-------|------|
| FILE SIZE | 5 MB | 200 MB | 2 GB |
| BUF SIZE | 128 B | 1024 B | 10000 B |

### GRAPH RESULTS 


### **SMALL SIZE : 5 MB**
![small](https://github.com/Ropho/HA_3sem/blob/main/task3/code/1.png)
<br/>

### **MIDDLE SIZE : 200 MB**
![med](https://github.com/Ropho/HA_3sem/blob/main/task3/code/2.png)
<br/>

### **BIG SIZE : 2 GB**
![big](https://github.com/Ropho/HA_3sem/blob/main/task3/code/3.png)


### POST-ANALYSIS

Easiest and fastest way to transfer data is **SHARED MEMMORY** ipc.


As for **MQ**, it shows average better results than **FIFO** but not quite easy to implement as **FIFO** and not so fast as **SHARED MEMMORY**.
