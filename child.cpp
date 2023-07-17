#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define NUM_CHILDREN 3

void interruptHandler(int signum)
{
    printf("Child received interrupt signal %d\n", signum);
}

int main()
{
    int i;
    pid_t pid;
    int fd[NUM_CHILDREN][2];

    // Create pipes for communication
    for (i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(fd[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    // Fork into child processes
    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            close(fd[i][1]); // Close the write end of the pipe
            signal(SIGINT, interruptHandler);

            char message[100];
            read(fd[i][0], message, sizeof(message));
            printf("Child %d received message: %s\n", i + 1, message);

            close(fd[i][0]); // Close the read end of the pipe
            exit(0);
        } else {
            // Parent process
            close(fd[i][0]); // Close the read end of the pipe

            char message[100];
            sprintf(message, "Message from parent to child %d", i + 1);
            write(fd[i][1], message, sizeof(message));

            close(fd[i][1]); // Close the write end of the pipe
        }
    }

    // Wait for all child processes to complete
    for (i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    return 0;
}

