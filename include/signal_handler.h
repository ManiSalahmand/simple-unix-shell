#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

/**
 * @brief Set up signal handlers used by the shell.
 *
 * Installs handlers for signals such as SIGINT so the shell behaves
 * correctly in interactive mode (e.g., Ctrl+C does not terminate
 * the shell itself).
 *
 * @return int 0 on success, -1 on failure.
 */
int setup_signal_handlers(void);

#endif // SIGNAL_HANDLER_H
