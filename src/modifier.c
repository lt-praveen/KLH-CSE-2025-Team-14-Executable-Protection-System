#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t attempt_started = 0;

void handle_signal(int sig)
{
    if (sig == SIGUSR1)
    {
        attempt_started = 1;
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
        return 1;
    }

    /*
     * Install SIGUSR1 handler.
     * The controller uses this signal to tell the
     * modifier process when to perform the test.
     */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    printf("[MODIFIER] PID: %d\n", getpid());
    printf("[MODIFIER] Waiting for SIGUSR1...\n");
    fflush(stdout);

    /*
     * Wait until the controller sends SIGUSR1.
     */
    while (!attempt_started)
    {
        pause();
    }

    printf("[MODIFIER] Received SIGUSR1.\n");
    printf("[MODIFIER] Attempting to open '%s' for writing...\n",
           argv[1]);

    /*
     * Try to open the executable for writing.
     *
     * If the executable is currently running,
     * Linux is expected to reject this operation
     * with ETXTBSY.
     */
    int fd = open(argv[1], O_WRONLY);

    if (fd == -1)
    {
        printf("[MODIFIER] open() FAILED.\n");
        printf("[MODIFIER] errno = %d\n", errno);
        printf("[MODIFIER] Error: %s\n", strerror(errno));

        if (errno == ETXTBSY)
        {
            printf("[MODIFIER] Result: Executable is currently "
                   "being executed (ETXTBSY).\n");
        }

        return 0;
    }

    printf("[MODIFIER] open() SUCCEEDED.\n");

    /*
     * This is reached if the executable could be opened.
     * We attempt a small write for completeness.
     */
    const char data[] = "TEST";

    ssize_t written = write(fd, data, sizeof(data) - 1);

    if (written == -1)
    {
        printf("[MODIFIER] write() FAILED.\n");
        printf("[MODIFIER] errno = %d\n", errno);
        printf("[MODIFIER] Error: %s\n", strerror(errno));
    }
    else
    {
        printf("[MODIFIER] write() SUCCEEDED: %zd bytes written.\n",
               written);
    }

    close(fd);

    return 0;
}
