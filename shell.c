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

    int pfds[2];

    pipe(pfds);
    pid_t pid1 = fork();
     

    if (pid1 == 0){
      close(pfds[0]);
      
      // Redirect stdout to current pipe
      dup2(pfds[1], STDOUT_FILENO);
      close(pfds[1]);
      
      
      // Start command
      execvp(args1[0], args1);
      
      printf("*** Command not found: %s\n", args1[0]);
      exit(1);
    }
    else if (pid1 > 0){
      int status = 0;
      waitpid(pid1, &status, 0);
      if (WEXITSTATUS(status) == 0)
	printf("*** Child exited sucessfully\n");
      else
	printf("*** Child exited with %d\n", WEXITSTATUS(status));
      
      pid_t pid2 = fork();

      if (pid2 == 0){
	// Close write end of current pipe (not needed in the parent)
	close(pfds[1]);

	// Start last command
	printf("*** Second command:");
	for (i = 0; args2[i] != NULL; i++)
	  printf(" %s", args2[i]);
	printf(" (%d words)\n", i);

	if (args2[0] == NULL) {
        printf("*** Empty command\n");
        return;
	}

	// Get stdin from last pipe
	dup2(pfds[0], STDIN_FILENO);
	close(pfds[0]);
   
	execvp(args2[0], args2);
    
	printf("*** Command not found: %s\n", args2[0]);
	exit(1);
      }
      else if (pid2 > 0){
	close(pfds[0]);
	close(pfds[1]);

	int status = 0;
	waitpid(pid2, &status, 0);
	if (WEXITSTATUS(status) == 0)
	  printf("*** Child exited sucessfully\n");
	else
	  printf("*** Child exited with %d\n", WEXITSTATUS(status));
      }
      else
	printf("*** Fork failed\n");
    }
    else
      printf("*** Fork failed\n");
}

void executeN(char* const argsvN[][MW], int num){
  size_t i;
  int prev_pipe, pfds[2];

  prev_pipe = STDIN_FILENO;

  for (i = 0; i < num; i++) {
    printf("*** Command #%ld:", i+1);
    int j;
    for (j = 0; argsvN[i][j] != NULL; j++)
        printf(" %s", argsvN[i][j]);
    printf(" (%d words)\n", j);

    if (argsvN[i][0] == NULL) {
        printf("*** Empty command\n");
        return;
    }
    pipe(pfds);

    pid_t pid = fork();
    if (pid == 0) {
      // Redirect previous pipe to stdin
      if (prev_pipe != STDIN_FILENO) {
	dup2(prev_pipe, STDIN_FILENO);
	close(prev_pipe);
      }

      if (i<num-1)
	// Redirect stdout to current pipe
	dup2(pfds[1], STDOUT_FILENO);
      close(pfds[1]);
      
      // Start command
      execvp(argsvN[i][0], argsvN[i]);
      
      printf("*** Command not found: %s\n", argsvN[i][0]);
      exit(1);
    }
    else if(pid > 0){
      if(prev_pipe != STDIN_FILENO)
	close(prev_pipe);
      int status = 0;
      waitpid(pid, &status, 0);
      if (WEXITSTATUS(status) == 0)
	printf("*** Child exited sucessfully\n");
      else
	printf("*** Child exited with %d\n", WEXITSTATUS(status));
      
      close(pfds[1]);
      // Save read end of current pipe to use in next iteration
      prev_pipe = pfds[0];
    }
    else
      printf("*** Fork failed\n");
  }
  close(pfds[0]);
}
