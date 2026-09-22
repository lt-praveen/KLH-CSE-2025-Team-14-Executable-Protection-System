#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t running = 1;

void handle_termination(int sig)
{
    (void)sig;
    running = 0;
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_termination;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGTERM, &sa, NULL);

    printf("[PROTECTED_APP] Started successfully.\n");
    printf("[PROTECTED_APP] PID: %d\n", getpid());
    printf("[PROTECTED_APP] Executable is now running.\n");
    fflush(stdout);

    /*
     * Keep the executable running so that the modifier
     * process can attempt to modify it.
     */
    while (running)
    {
        sleep(1);
    }

    printf("[PROTECTED_APP] Terminating normally.\n");

    return 0;
}
