# That directory contains the 3'rd homework for out computer science lessons

The program is created to measure and compare time that different ipc's takes for transfering file from one procces to another. 

# How to build

Let's build fifo for example

```terminal    
  $ cd HW/FIFO    
  $ make    
  $ ./snd filename
  $ ./rcv
  ```
  
  After launching program from terminal using ./snd you have to open another terminal for launching receiver by ./rcv
  
  But in case of shared memory you should launch receiver first
   
  Then you'll receive read data in output.txt file that will be placed in HW dir.
  
  All measures was provided using file.txt that was automatically generated via 
  
```terminal
  $ time dd if=/dev/urandom of=file.txt bs=8192 count=4096  
  ```
  
  All measured data placed in log.txt      
  
  Plot layout:
  
    Blue data was gotten by using FIFO ipc
    Black data was gotten by using Shared memory ipc
    Orange data was gotten by using Massege Queue ipc

**Have fun!**     
![Alt-текст](https://i.pinimg.com/originals/11/4a/f8/114af8f8aea2294641f950ed3dd85f46.jpg)
