rshell
======

Project source can be downloaded from https://github.com/jesuscreyes/rshell.git
----

Author & Contributor List
-------------------------
Jesus Reyes

All other known bugs and fixes can be sent to jesuscreyes11@gmail.com

Reported bugs/fixes will be submited to correction.
 

File List
---------

`Makefile`

`LICENSE`

`README.md`

`./src`

-`hw0.cpp`

-`ls.cpp`

-`cp.cpp`

-`hw2.cpp`

-`hw3.cpp`

`./tests`

To clone the repository, follow the directions below:
---------------
1. Enter: `git clone http://github.com/jesuscreyes/rshell.git`


Once you have cloned the repository to your local machine, follow the directions below:
---------------

homework 0:

1. Enter: `cd rshell`

2. Enter: `git checkout hw0`

3. Enter: `make`

4. Enter: `bin/rshell`

homework 1:

1. Enter: `cd rshell`

2. Enter: `git checkout hw1`

3. Enter: `make`

4. Enter: `bin/ls` for executing ls.cpp or `bin/cp` for executing cp.cpp.

homework 2:

1. Enter: `cd rshell`

2. Enter: `git checkout hw2`

3. Enter: `make`

4. Enter: `bin/rshell`

homework 3:

1. Enter: `cd rshell`

2. Enter: `git checkout hw3`

3. Enter: `make`

4. Enter: `bin/rshell`

Overview of program
-------------------

hw0.cpp: This program is intended to replicate the functionality of a command shell.

Features:
- Can execute basic commands such as: `ls`, `echo`, and `mkdir`
- Can execute multiple commands connected by connecter `;`, `&&`, or `||`
- Can be exited by typing exit[ENTER]

ls.cpp: This program is intended to replicate the functionality of the `ls` command

Features:
- Can output all files in current directory
- Can output accordingly when `-a` and `-l` flags
- Directories are displayed blue
- Executables are displayed green
- Hidden files have a gray background

cp.cpp: This program is intended to replicate the functionality of the `cp` command

Features:
- When called with no extra parameters, then files are copied with most efficient method, using `BUFSIZ`.
- Includes OPTIONAL PARAMETER: `-f`
- OPTIONAL PARAMETER can be placed anywhere in the argument list
- When the optional parameter is called, program copies the file using each of the three methods, times how long each method takes, and report the results to stdout.
- Times that are shown include: wallclock run time, user run time, and system runtime.

hw2.cpp: This program is intended to mimic i/o redirection and piping from a bash terminal.

Features:
- Input Redirection `<`
- Output Redirection `>` `>>`
- Piping `|`
- Can combine commands with pipes and i/o redirection
- Can be exited by typing `exit`[ENTER]

hw3.cpp: This program adds features on top of the command shell program.

Features:
- Uses `PATH` variable when executing functions
- Handles `^C` interrupts
- `cd` command is implemented
- Prompt now displays current working directory

Bugs & Limitations
-----------------

hw0.cpp:

1. '&&' and '||' are restricted to the same behavior as ';' in this version.

2. Connecters (e.g. '&&', '||', ';') cannot be used together in command (e.g. $ pwd && ls ; pwd || echo hello world).

3. Does not handle case where input is long enough to overtake memory.

4. Rshell outputs error message when only whitespace is inputted, instead of outputting nothing and asking for next input.

5. Rshell tries to execute `exit` when it is followed by a comment in input (e.g. `$exit    #Comment`), instead of exiting the program.

ls.cpp:

1. Output is listed based on when it is created as opposed to alphabetical order

2. Output does not line up exactly as it should

3. Calling program on directories does not work

4. Calling program using `-R` flag does not work

hw2.cpp:

1. Ignores commands that include repeated instances of input or output redirection.
- e.g. `cat < inputFile < outputFile` will output the contents of `inputFile`, and not `outputFile`.

2. If multiple redirections are called and are not separated by a pipe, only the first instance of redirection will be exexecuted. e.g. `cat < inputFile > outputFile` will output the contents of in and not put the contents of `inputFile` into the file `outputFile..

3. Cannot handle string input redirection  `<<<`

4. Cannot perform output redirection on whatever file descriptor
- e.g. `g++ main.cpp 2> errors`

5. Grep command not working with pipes.

6. Sed command not working with pipes.

hw3.cpp:

1. Does not handle multiple arguments or flags for `cd` command.
2. Does not handle `cd ~` or `cd` by itself.
