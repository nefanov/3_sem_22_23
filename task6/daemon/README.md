# DEMON  

>Program is monitoring changes in text files in the direcotry of process, which pid was inputed as the start parameter  
Daemon works in two modes 

1. **Interactive**  

To launch the program in interactive mod use

```terminal 
  ./daemon <pid> -i
```

if daemon was launched in interective mode you should start interface program.

```terminal
  ./interface
```

And then input the daemon pid into the interface. No matter was it launched interactive or not
daemon will print it's pid.

You can send some commands to daemon:

```
  set_t #sets daemon sleep time
  print <k> #prints k diffs into terminal
  stop #stops daemon
```

2. **Daemon**  


To launch the program in daemon mod use

```terminal 
  ./daemon <pid> -d
``` 
In this mode daemon only monitors directory of the process and periodically saves diffs into .patch files
  
In order to kill daemon use 

```
  $ kill -30 <daemonpid>
  $ rm ./daemon/lucifer.fifo
```

**Have fun!**     
![Alt-текст](https://ondemand.spectrum.net/static/15a5713cf9251b1d9827e6f621a9f2c2/2fda2/SH029209900000.jpg)
