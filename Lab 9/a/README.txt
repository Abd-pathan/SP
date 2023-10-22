This C program demonstrates signal handling and child process control.
Basically there is a child process and signal handlers for SIGINT, SIGTSTP, and SIGQUIT. 
When SIGINT (Control-C) is received, the child process is suspended, and the parent process waits. 
When SIGTSTP (Control-Z) is received, it performs the same suspension. 
Pressing SIGQUIT (Control-\) terminates the entire program. 