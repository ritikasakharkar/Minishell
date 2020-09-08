# Minishell 

- Shell is the command interpreter in the linux platform. Linux shell commands provides us the platform to perform operation over the terminal. 

- We used the Linux System Calls and commands for file and process management to implement the functionalities of shell. Our program will behave similarly to the Linux shell and wil display a prompt (i.e minishell>> ) at start-up.

- Our minishell can accept and execute the following commands which is discussed in details below under implementation title.
 
    -ren(rename the file

    -del (delete the file)

    -copy(copy the file)

    -dir(list the files in directory)

    -exit(exit our shell)

- When we provide any command to the shell, the shell reads the command from the input and provides the obtained input to the parser which is followed by the execution of the standard input. 

- When the input is passed for the parsing, the command string is separated into a program and arguments. 

- To run the code:-
 
    -> gcc new.c -lpthread -o ./newfile.out

    -> ./newfile.out
