For the homework 2 I have completed all the given requirements.

Flow and explanation of code -

After the header files functional pointer, its initialization and function prototypes are declared.

In the main function, after declaration of all the required varibles, i have used 'getopt' because we wanted to pass the parameters to program at runtime from Command Line Interface (CLI).

After that the 'argc' and 'argv' are used to hold the value which is passed on CLI, which is then stored into 'optarg'.And then the 'optind'.
All of these are used to take the user input and store them in local variables through which the program further proceeds.

Then I have passed all the values to function 'myFunc' where the actual coding begins.

The function myFunc firstly handle errors with 'opendir' and traverse through each directory using 'readdir'.
Then I have biuld the path for each entry.

The entry is classified using different things like - depth traversal and the fileI and fileCheker functions. The output is print according to results of these.

Basically the functions fileI and fileCheker checks if the path is of a file or directory.
To print the details like permissions,timestamp etc of file I have implemented 'stat'.

To be precise,st_mode is used check the bit sets of S-ISREG and S_ISDIR.

The functions fileI and fileCheker return 1 if it is file or directory otherwise they return 0. 