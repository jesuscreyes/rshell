rshell
======

Licensing Information: READ LICENSE
---
Project source can be downloaded from https://github.com/jesuscreyes/rshell.git
----

Author & Contributor List
-------------------------
Jesus Reyes

Mike Izbicki

All other known bugs and fixes can be sent to jreye028@ucr.edu

Reported bugs/fixes will be submited to correction.
 

File List
---------

`Makefile

LICENSE

README.md
`

`./src

main.cpp`

./tests



Once you have cloned the directory to your local machine, follow the directions below:
---------------
1. `cd` into `rshell` directory
2. Enter `git checkout hw0`
3. Enter `make`
4. Enter `bin/rshell`

Overview of program
-------------------
This program is intended to replicate the functionality of a command shell.

Features:
- Can execute basic commands such as: `ls`, `echo`, and `mkdir`
- Can execute multiple commands connected by connecter `;`, `&&`, or `||`
- Can be exited by typing exit[ENTER]

Bugs & Limitations
-----------------
1. '&&' and '||' are restricted to the same behavior as ';' in this version.
2. Does not handle case where input is long enough to overtake memory.
3. Rshell tries to execute whitespace when it is followed by a comment in input. (e.g. `$     #Comment`)
4. Rshell tries to execute `exit` when it is followed by a comment in input (e.g. `$exit    #Comment`), instead of exiting the program.
