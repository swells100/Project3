#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
  for (size_t i = 0; i < N; i++) {
    if (strcmp(cmd, allowed[i]) == 0) {
      return 1;
    }
  }
	
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *args[10];
    posix_spawnattr_t attr;

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

    int arg_count = 0;
    char *token = strtok(line, " ");
    while (token != NULL && arg_count < 9) {
      args[arg_count++] = token;
      token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (!isAllowed(args[0])) {
      printf("NOT ALLOWED!\n");
      continue;
    }

    if (strcmp(args[0], "exit") == 0) {
      break;
    } else if (strcmp(args[0], "help") == 0) {
      printf("The allowed commands are:\n1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");
      continue;
    } else if (strcmp(args[0], "cd") == 0) {
      /*if (args[1] == NULL) {
        fprintf(stdout, "cd: missing argument\n");
      } else*/ if (args[2] != NULL) {
        fprintf(stdout, "cd: too many arguments\n");
      } else {
        if (chdir(args[1]) != 0) {
          perror("cd failed");
        }
      }
      continue;
    }

    pid_t pid;
        int status;
        
        posix_spawnattr_init(&attr);

        // Spawn a new process
    if (posix_spawnp(&pid, args[0], NULL, &attr, args, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    // Wait for the spawned process to terminate
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
      int exit_status = WEXITSTATUS(status);
    if (exit_status != 0) {
        printf("Spawned process exited with status %d\n", WEXITSTATUS(status));

    }
    }
 

        posix_spawnattr_destroy(&attr);



	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.
    /* EXAMPLE CODE FROM MYSPAWN: pid_t pid;
    char *argv[] = {"echo", "Hello from the spawned process!", NULL};
    int status;
    posix_spawnattr_t attr;

    // Initialize spawn attributes
    posix_spawnattr_init(&attr);

    // Set flags if needed, for example, to specify the scheduling policy
    // posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    // Spawn a new process
    if (posix_spawnp(&pid, "echo", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    // Wait for the spawned process to terminate
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
    }

    // Destroy spawn attributes
    posix_spawnattr_destroy(&attr);

    return EXIT_SUCCESS; */

    }
    return 0;
}
