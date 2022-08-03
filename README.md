# Shell-Implementation

Project Description:

  In this assignment, you will implement a command line interpreter, or shell, on top of Unix. The shell should operate in this basic way: when you type 
  in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input 
  when it has finished.  More specifically, shells are typically implemented as a simple loop that waits for input and fork()s a new child process to 
  execute the command; the child process then exec()s the specified command while the parent process wait()s for the child to finish before continuing 
  with the next iteration of the loop. The shell you implement will be similar to, but much simpler, than the one you run every day in Unix. You can 
  find out which shell you are running by typing echo $SHELL at a prompt. You may then wish to look at the man pages for sh or the shell you are running 
  to learn more about all of the functionality that can be present. For this project, you do not need to implement as much functionality as is in most 
  shells.  Besides the most basic function of executing commands, your shell (mysh) must provide the following three features: interactive vs. batch mode,
  output redirection, and aliasing.
  
How to Run:
  1) Download files onto your local machine
  2) Run the Makefile in your filepath by simply typing "Make" in the terminal.
  3) To run in interactive mode, type "./mysh", then run any command by starting with "/bin/[insert command here]" followed by the comnmand in place of 
  [insert command here]. ex: /bin/ls
  4) To run in batch mode, type "./mysh [batch-file]" by replacing [batch-file] with the file of commands you would like to pass through.
  
Notes: 
  1) To redirect your output, simply use ">" after the command followed by the output file. ex: /bin/ls -la /tmp > output
  2) to create an alias, type "alias [desired shortcut] [actual command]". ex: alias ls /bin/ls
