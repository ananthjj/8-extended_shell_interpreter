#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute1(char* const args[])
{
    int i;
    printf("*** Command:");
    for (i = 0; args[i] != NULL; i++)
        printf(" %s", args[i]);
    printf(" (%d words)\n", i);

    if (args[0] == NULL) {
        printf("*** Empty command\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        printf("*** Command not found: %s\n", args[0]);
        exit(1);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) == 0)
            printf("*** Child exited sucessfully\n");
        else
            printf("*** Child exited with %d\n", WEXITSTATUS(status));
    } else {
        printf("*** Fork failed\n");
    }
}

void execute2(char* const args1[], char* const args2[])
{
    int i;
    printf("*** First command:");
    for (i = 0; args1[i] != NULL; i++)
        printf(" %s", args1[i]);
    printf(" (%d words)\n", i);

    if (args1[0] == NULL) {
        printf("*** Empty command\n");
        return;
    }

    int prev_pipe, pfds[2];

    prev_pipe = STDIN_FILENO;

    pipe(pfds);
    pid_t pid = fork();

    if (pid == 0) {
      // Redirect previous pipe to stdin
      if (prev_pipe != STDIN_FILENO) {
	dup2(prev_pipe, STDIN_FILENO);
	close(prev_pipe);
      }
      
      // Redirect stdout to current pipe
      dup2(pfds[1], STDOUT_FILENO);
      close(pfds[1]);
      
      // Start command
      execvp(args1[0], args1);
      
      perror("execvp failed");
      exit(1);
    }

    // Close read end of previous pipe (not needed in the parent)
    close(prev_pipe);
    
    // Close write end of current pipe (not needed in the parent)
    close(pfds[1]);
    
    // Save read end of current pipe to use in next iteration
    prev_pipe = pfds[0];

    // Start last command
    printf("*** Second command:");
    for (i = 0; args2[i] != NULL; i++)
        printf(" %s", args2[i]);
    printf(" (%d words)\n", i);
    
    // Get stdin from last pipe
    if (prev_pipe != STDIN_FILENO) {
      dup2(prev_pipe, STDIN_FILENO);
      close(prev_pipe);
    }
    
    execvp(args2[0], args2);
    
    perror("execvp failed");
    exit(1);   
}
