#include <signal.h>
#include <unistd.h>

#include "signal_handler.h"

/**
 * @brief Handle SIGINT in the interactive shell process.
 *
 * @param signo The received signal number.
 */
static void handle_sigint(int signo)
{
    (void)signo;
    write(STDOUT_FILENO, "\n", 1);
}

/**
 * @brief Configure signal handlers used by the shell.
 *
 * @return int 0 on success, -1 on failure.
 */
int setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
        return -1;

    return 0;
}
