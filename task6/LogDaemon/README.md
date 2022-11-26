# Diff Daemon

## Description

Program saves 


### Version 0.0

* Read working directory of process(pid's hardcoded)
* Save all files

### Version 0.1

* Change files in CWD
* Save diffs (name=timestamp)

### Version 0.2

* Save diffs every T(value hardcoded) secs

### Version 0.3

* Launch in daemon mode
* Cmd args

### Version 0.4

* Create named FIFO to talk with daemon
* Add API:
	* set T
	* set PID
	* exit
	* print current diff
	* restore condition by patching diffs

### Version 0.5
* Provide only one daemon existence

### Features
* print last K diffs
* change observable process (PID)


### Version X

Incremental backup'er with interaction via FIFO.  Can work in interactive and in daemon modes.