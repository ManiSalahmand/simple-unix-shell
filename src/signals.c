#include <signal.h>
#include <unistd.h>

#include "signal_handler.h"

/**
 * @brief Handle SIGINT in the interactive shell process.
 *
 * This prevents the shell itself from terminating when the user presses
 * Ctrl+C. Instead it simply prints a newline so the prompt can be shown
 * again on the next loop iteration.
 *
 * @param signo The received signal number.
 */
static void handle_sigint(int signo)
{
    (void)signo; // suppress unused parameter warning
    write(STDOUT_FILENO, "\n", 1);
}

/**
 * @brief Configure signal handlers used by the shell.
 *
 * Installs a custom handler for SIGINT so the shell remains running
 * when the user presses Ctrl+C.
 *
 * @return int 0 on success, -1 if sigaction() fails.
 */
int setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask); // no additional signals blocked during handler
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
        return -1;

    return 0;
}
