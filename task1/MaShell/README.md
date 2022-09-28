## MaSHell

### Description

Simple shell-like program for MIPT OS course

### Features

* PATH-sensitive (can launch any program which path belongs to PATH environment variable)
* Uses pipes for IPC (inter process communication) [weeeeird thing]
* Based on using of fork and execvpe system aclls
* It's possible to launch MaShell in MaShell in MaShell and so on :P
* Supports input/output redirection with '|'

### Build

To build and run program use:

```
    $ make
    $ ./MaShell.exe
```

To start with valgrind:
```
   $ make
   $ make valgrind
```