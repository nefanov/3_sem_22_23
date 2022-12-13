# Diff Daemon

- [Diff Daemon](#diff-daemon)
	- [Description](#description)
		- [What is it?](#what-is-it)
		- [What does it do?](#what-does-it-do)
		- [How to interact with daemon?](#how-to-interact-with-daemon)
		- [Build](#build)
	- [Roadmap](#roadmap)
		- [Version 0.0](#version-00)
		- [Version 0.1](#version-01)
		- [Version 0.2](#version-02)
		- [Version 0.3](#version-03)
		- [Version 0.4 \[@ currently here\]](#version-04--currently-here)
		- [Version 0.5](#version-05)
		- [Features](#features)

## Description

#### What is it?
* It's a [SysV daemon](https://www.freedesktop.org/software/systemd/man/daemon.html) program.

#### What does it do?
* Track CWD (work dir) changes of set process.
* Save incremental diffs of changed text (once in T sec).
* Execute commands (check [Daemon API](#how-to-interact-with-daemon))
* Ignore signals (SIGINT, SIGQUIT...)


#### How to interact with daemon?

Communication with daemon is implemented with named FIFO.
It's name is set in [LogDaemon.h](LogDaemon.h). By default it is *"/tmp/LogDaemon.fifo"*.

Daemon has an API:

```
- set_T <time in secs> // set polling time

- print_k <k> // print in DAEMON_TERMINAL k last diffs

                // by default print to ./info/LogDaemon_terminal.txt

- restore_k <k> // restores files k diffs back

- stop // stops daemon
```

To interact you should write command to FIFO. For example:

```
$ echo "set_T 10" > /tmp/LogDaemon.fifo
```


#### Build

Build it:
```
$ cd LogDaemon && make
```

Then start daemon (sudo! needed for syscalls like *readsymlink*):
```
$ sudo ./LogDaemon <mode> <pid>
```

\<pid\>: pid of followed process 

\<mode\>:

        -i: interactive
        -d: daemon


*By default mode is interactive*

As interaction is made with FIFO, you **have to** open writing side of FIFO:
```
$ echo "set_T 10" > /tmp/LogDaemon.fifo
```

Congratulations! Now LogDaemon is alive (at least should be). It should save diffs in SAMPLE_DIR (*./daemon_sample*)

## Roadmap

#### Version 0.0

* Read working directory of process(pid's hardcoded)
* Save all files

#### Version 0.1

* Change files in CWD
* Save diffs (name=timestamp) using inotify

#### Version 0.2

* Save diffs every T(value hardcoded) secs

#### Version 0.3

* Launch in daemon mode
* Cmd args

#### Version 0.4 [@ currently here]

* Create named FIFO to talk with daemon
* Add API:
	* set T
	* exit
	* print current diff
	* restore condition by patching diffs

#### Version 0.5
* Provide only one daemon existence

#### Features

* Move through history
* set PID
...
