#!/usr/bin/env python3
import os
import pty
import signal
import subprocess
import time

BUILD_SHELL = os.path.join('.', 'build', 'shell')


def read_until(fd: int, expected: bytes, timeout: float = 2.0) -> bytes:
    """
    Read data from a file descriptor until a specific byte sequence appears.

    Arguments:
        fd: File descriptor to read from.
        expected: Byte sequence to wait for in the output.
        timeout: Maximum time in seconds to wait before failing.

    Returns:
        bytes: Data read from the file descriptor including the expected sequence.
    """
    data: bytes = b''
    end: float = time.time() + timeout

    while expected not in data:
        if time.time() > end:
            raise TimeoutError(f"Timed out waiting for {expected!r}; got {data!r}")

        chunk: bytes = os.read(fd, 1024)
        if not chunk:
            break

        data += chunk

    return data


def test_sigint_at_prompt() -> None:
    """
    Verify that pressing Ctrl+C (SIGINT) at the shell prompt does not
    terminate the shell and that a new prompt is displayed.
    """
    master_fd: int
    slave_fd: int
    master_fd, slave_fd = pty.openpty()

    proc: subprocess.Popen[bytes] = subprocess.Popen(
        [BUILD_SHELL],
        stdin=slave_fd,
        stdout=slave_fd,
        stderr=slave_fd,
        preexec_fn=os.setsid, # start shell in its own process group
        close_fds=True
    )

    os.close(slave_fd)

    try:
        output: bytes = read_until(master_fd, b'shell> ')
        os.killpg(proc.pid, signal.SIGINT)
        output += read_until(master_fd, b'shell> ')

        if output.count(b'shell> ') < 2:
            raise AssertionError(f"shell did not reprompt after SIGINT: {output!r}")

        proc.terminate()
        proc.wait(timeout=1)

    finally:
        try:
            proc.kill()
        except ProcessLookupError:
            pass

        os.close(master_fd)


def test_sigint_interrupts_child_not_shell() -> None:
    """
    Verify that SIGINT interrupts a running child process but does not
    terminate the shell itself.
    """
    master_fd: int
    slave_fd: int
    master_fd, slave_fd = pty.openpty()

    proc: subprocess.Popen[bytes] = subprocess.Popen(
        [BUILD_SHELL],
        stdin=slave_fd,
        stdout=slave_fd,
        stderr=slave_fd,
        preexec_fn=os.setsid, # allow SIGINT to be sent to the whole shell process group
        close_fds=True
    )

    os.close(slave_fd)

    try:
        read_until(master_fd, b'shell> ')
        os.write(master_fd, b'sleep 5\n') # launch a long-running child command
        time.sleep(0.2) # give the child time to start before sending SIGINT
        os.killpg(proc.pid, signal.SIGINT)
        read_until(master_fd, b'shell> ')

        if proc.poll() is not None:
            raise AssertionError("shell exited after SIGINT")

    finally:
        try:
            proc.kill()
        except ProcessLookupError:
            pass

        os.close(master_fd)
