#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define EXECUTABLE "./protected_app"

int main(void)
{
    pid_t protected_pid;
    pid_t modifier_pid;

    int pipefd[2];

    printf("============================================\n");
    printf("   EXECUTABLE PROTECTION SYSTEM\n");
    printf("============================================\n\n");

    /*
     * Create an anonymous pipe.
     *
     * CO-3:
     * The pipe is used for communication between
     * the controller and protected process.
     */
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    /*
     * Create the protected process.
     *
     * CO-2:
     * Demonstrates process creation using fork().
     */
    protected_pid = fork();

    if (protected_pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (protected_pid == 0)
    {
        /*
         * Child process.
         *
         * CO-1:
         * exec() replaces the child process with
         * the protected executable.
         */

        close(pipefd[0]);

        char message = 'R';

        /*
         * Notify the parent that the child has started.
         */
        if (write(pipefd[1], &message, 1) == -1)
        {
            perror("write");
            exit(1);
        }

        close(pipefd[1]);

        execl(EXECUTABLE, EXECUTABLE, (char *)NULL);

        /*
         * execl() only returns if an error occurs.
         */
        perror("execl");
        exit(1);
    }

    /*
     * Parent process.
     */

    close(pipefd[1]);

    printf("[CONTROLLER] Protected process created.\n");
    printf("[CONTROLLER] Protected PID: %d\n", protected_pid);

    /*
     * Wait for the child to notify us through the pipe.
     */
    char message;

    if (read(pipefd[0], &message, 1) == -1)
    {
        perror("read");
        return 1;
    }

    close(pipefd[0]);

    printf("[CONTROLLER] Protected process is ready.\n\n");

    /*
     * Give the protected executable a moment to
     * finish loading and enter its main loop.
     */
    sleep(1);

    /*
     * Create the modifier process.
     *
     * CO-2:
     * Another child process is created using fork().
     */
    modifier_pid = fork();

    if (modifier_pid == -1)
    {
        perror("fork");
        kill(protected_pid, SIGTERM);
        waitpid(protected_pid, NULL, 0);
        return 1;
    }

    if (modifier_pid == 0)
    {
        /*
         * Child process becomes modifier program.
         *
         * CO-1:
         * exec() launches modifier.
         */
        execl("./modifier",
              "./modifier",
              EXECUTABLE,
              (char *)NULL);

        perror("execl modifier");
        exit(1);
    }

    printf("[CONTROLLER] Modifier process created.\n");
    printf("[CONTROLLER] Modifier PID: %d\n\n", modifier_pid);

    /*
     * Give modifier time to install its signal handler.
     */
    sleep(1);

    printf("[CONTROLLER] Sending SIGUSR1 to modifier...\n");

    /*
     * CO-3:
     * Signal-based IPC / asynchronous notification.
     */
    if (kill(modifier_pid, SIGUSR1) == -1)
    {
        perror("kill");
    }

    /*
     * Wait for modifier to finish its test.
     *
     * CO-2:
     * Process synchronization using waitpid().
     */
    int modifier_status;

    if (waitpid(modifier_pid, &modifier_status, 0) == -1)
    {
        perror("waitpid");
    }

    printf("\n[CONTROLLER] Modifier process completed.\n");

    /*
     * Terminate the protected executable.
     */
    printf("[CONTROLLER] Sending SIGTERM to protected process...\n");

    if (kill(protected_pid, SIGTERM) == -1)
    {
        perror("kill");
    }

    /*
     * Wait for protected process termination.
     */
    int protected_status;

    if (waitpid(protected_pid, &protected_status, 0) == -1)
    {
        perror("waitpid");
    }

    printf("[CONTROLLER] Protected process terminated.\n");

    printf("\n============================================\n");
    printf("              TEST COMPLETED\n");
    printf("============================================\n");

    return 0;
}
