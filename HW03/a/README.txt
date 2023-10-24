The code of homework 3 is basically an updated version of homework 2.
In this we have implemented 2 more options '-e' and '-E'.

The -e option allows you to execute a single command for each matching file. For example, if you want to List all files that have the substring “jpg” in their
filename with depth <=3 relative to the current
directory and size <= 1024, and execute the command
"wc -l" on each file , you can use the following command: 
./search -s 1024 -f jpg 3 -e "wc -l"

The -E option allows you to execute a command with a list of matching files as an argument. For example, you can create a tar archive of all text files in the specified directory with the following command:

./search-f txt 2 -E "tar cvf txt.tar"


implementation of -e:
In the code, the executeCommand function takes the user unput as the singleCommand argument. And then it processes each file in the specified directory. When a file matches the criteria (e.g., based on size and pattern), it executes the command provided. It replaces {} in the command with the current file's path.The utility constructs the full command by combining the singleCommand and the file path.
The system function is then used to execute the constructed command. It runs the command in the shell.
The code captures the exit status of the command and checks if it's successful (exit status 0) or if it failed.


implementation of -E:
The executeCommand function is also used for this option, but with a slight difference. It takes two command strings: singleCommand and fileListCommand.

When a file matches the criteria (e.g., based on size and pattern), the utility collects the file paths and creates a list.After processing all matching files, it then executes the fileListCommand once, and it appends the list of file paths as arguments to this command.

The system function is used to run the constructed command, and the exit status is captured and checked for success or failure.

