/*
 * Forker - Linux Command Line Forking Tool
 *
 * Author: fuggles-5555
 *
 * Description:
 *   Forker is a command-line tool designed for parallel execution of commands on Linux.
 *   Users can supply a file containing a list of commands as the first argument and
 *   specify the maximum number of threads for concurrent execution as the second argument.
 *
 * Compilation:
 *   gcc forker.c -o forker
 *
 * Usage:
 *   ./forker <command_file> <max_threads>
 *
 * Example:
 *   ./forker commands.txt 4
 *
 * For more information and updates, visit: https://github.com/fuggles-5555/forker
*/

// libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// constants
#define MAX_COMMAND_LENGTH 512
#define MAX_THREADS 100

// execute function
int executeCommand(char *command) {
    system(command);
    return 0;
}

// main function
int main(int argc, char *argv[]) {
    // count arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <cmdFile> <threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // check cmdFile is a file and open cmdFile
    const char *cmdFile = argv[1];
    FILE *cmdFilePtr = fopen(cmdFile, "r");
    if (!cmdFilePtr) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    // check threadCount is an integer and less then 100 
    int threadCount = atoi(argv[2]);
    if (threadCount <= 0 || threadCount > MAX_THREADS) {
        fprintf(stderr, "Invalid thread count. It should be between 1 and %d.\n", MAX_THREADS);
        exit(EXIT_FAILURE);
    }

    // declare other variables
    char command[MAX_COMMAND_LENGTH];
    int activeThreads = 0;

    // main loop
    while (fgets(command, MAX_COMMAND_LENGTH, cmdFilePtr) != NULL) {
        // remove the trailing newline character from the command   
        size_t len = strlen(command);           
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        
        // fork a child process
        pid_t pid = fork();

        // parent and child processes clone (sort of) and run from here
        // child has a pid of 0 and parent takes the child process pid
        
        // error forking
        if (pid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // child process
            executeCommand(command);
            _exit(EXIT_SUCCESS);
        } else {
            // parent process
            activeThreads++;
            // wait for a child process to complete when the thread count is reached
            if (activeThreads >= threadCount) {
                int status = 0;
                wait(&status);
                activeThreads--;
            }
        }
    }

    // wait for all remaining child processes to complete
    while (activeThreads > 0) {
        int status = 0;
        wait(&status);
        activeThreads--;
    }
    
    // close the file and exit
    fclose(cmdFilePtr);
    return 0;
}

