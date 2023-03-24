# gdb tips on mac 

## certificate 
if we want to use gdb on mac we have first to create a certification for gdb after it is installed successfully.

here are the steps of how to create a certification for gdb and use the system type certification to certificate the gdb 

1. find `Keychain Access` and go KeyChain Access's Certificate Assistant
2. then, create a Certification suppose we name the certification 'gdb_signer'
3. in last steps, select the system instead of login

after we generate the certification successfully, execute the command below 
```
> sudo killall taskgated 
> codesign -fs gdb_signer /usr/local/bin/gdb 
> type the password in the prompted dialog 
``` 

after everything is done, try to setup the gdb by 
```
> gdb + executable binary file

# trigger continue execution  
> run 
```

in my env, i got an error:
```
sh-3.2# gdb edb_server
GNU gdb (GDB) 13.1
Copyright (C) 2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-apple-darwin21.6.0".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from edb_server...
(No debugging symbols found in edb_server)
(gdb) run
Starting program: /Users/nanachi/db-engine/build/bin/edb_server
Note: this version of macOS has System Integrity Protection.
Because `startup-with-shell' is enabled, gdb has worked around this by
caching a copy of your shell.  The shell used by "run" is now:
    /var/root/Library/Caches/gdb/bin/sh
[New Thread 0x2703 of process 59459]
^C[New Thread 0x1803 of process 59459]
``` 

i have to kill the gdb process, and appending the content below to `~/.gdbinit` and restart the mac 
```
echo "set startup-with-shell off" >> ~/.gdbinit
```

## gdb command 
here are some gdb common commands frequently used during debugging
```
gdb edb_server 

> l --> show the code context 
> run --> continue with the debugging 
> break main --> add a breakpoing on main function
> step --> like the step into button in idea, helps to dive into the current's context's inner method
> n --> execute one line/execute line by line  
```
 
