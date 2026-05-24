#include "signal_ipc.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * run_worker - child process logic.
 *
 * The signal mask already has SIGUSR1, SIGUSR2, and SIGCHLD blocked
 * (set up in main() before fork). You do NOT need to call sigprocmask
 * again; just build a sigset_t for sigwait() and proceed.
 *
 * Required behavior:
 *  1. Build a sigset_t containing SIGUSR1 and SIGUSR2 for use with sigwait().
 *  2. Print "worker: ready" and flush stdout.
 *  3. Send SIGUSR1 to supervisor_pid to announce readiness.
 *  4. Wait for NUM_TASKS SIGUSR1 signals from the supervisor.
 *     For each one: print "worker: received task N" (N = 1, 2, 3),
 *     flush stdout, then send SIGUSR1 back to supervisor_pid as an ack.
 *  5. Wait for SIGUSR2 (the shutdown signal) from the supervisor.
 *  6. Print "worker: all tasks done, exiting" and flush stdout.
 *  7. Return 0.
 */
int run_worker(pid_t supervisor_pid) {
    sigset_t wait_mask;
    sigemptyset(&wait_mask);

    /* TODO: add SIGUSR1 and SIGUSR2 to wait_mask. */

    printf("worker: ready\n");
    fflush(stdout);

    /* TODO: send SIGUSR1 to supervisor_pid (readiness notification). */

    int tasks_done = 0;
    while (tasks_done < NUM_TASKS) {
        int sig = 0;

        /* TODO: call sigwait() with &wait_mask to receive the next signal
         *       into sig. Check the return value; return 1 on error. */

        if (sig == SIGUSR1) {
            tasks_done++;
            printf("worker: received task %d\n", tasks_done);
            fflush(stdout);

            /* TODO: send SIGUSR1 back to supervisor_pid as an acknowledgement. */
        }
    }

    /* TODO: call sigwait() once more to receive SIGUSR2 (the shutdown signal). */
    /* (You do not need to inspect which signal was received here.) */

    printf("worker: all tasks done, exiting\n");
    fflush(stdout);
    return 0;
}

/*
 * run_supervisor - parent process logic.
 *
 * The signal mask already has SIGUSR1, SIGUSR2, and SIGCHLD blocked.
 * Build a sigset_t for sigwait() and proceed.
 *
 * Required behavior:
 *  1. Build a sigset_t containing SIGUSR1 and SIGCHLD for use with sigwait().
 *  2. Wait for SIGUSR1 from the worker (readiness notification).
 *  3. For each task i = 1 .. NUM_TASKS:
 *       a. Print "supervisor: sending task i" and flush stdout.
 *       b. Send SIGUSR1 to worker_pid.
 *       c. Wait for SIGUSR1 ack from the worker with sigwait().
 *  4. Print "supervisor: shutting down worker" and flush stdout.
 *  5. Send SIGUSR2 to worker_pid (shutdown command).
 *  6. Wait for SIGCHLD with sigwait() to know the worker exited.
 *  7. Call waitpid() to reap the worker process.
 *  8. Print "supervisor: worker exited cleanly" and flush stdout.
 *  9. Return 0.
 */
int run_supervisor(pid_t worker_pid) {
    sigset_t wait_mask;
    sigemptyset(&wait_mask);

    /* TODO: add SIGUSR1 and SIGCHLD to wait_mask. */

    /* TODO: call sigwait() to wait for SIGUSR1 (worker ready notification).
     *       Discard the received signal number. Return 1 on error. */

    for (int i = 1; i <= NUM_TASKS; i++) {
        printf("supervisor: sending task %d\n", i);
        fflush(stdout);

        /* TODO: send SIGUSR1 to worker_pid. */

        /* TODO: call sigwait() to wait for SIGUSR1 ack from the worker. */
    }

    printf("supervisor: shutting down worker\n");
    fflush(stdout);

    /* TODO: send SIGUSR2 to worker_pid (shutdown command). */

    /* TODO: call sigwait() to wait for SIGCHLD (worker exit notification). */

    int status;
    /* TODO: call waitpid() to reap the worker. Use worker_pid, &status, 0. */
    (void)status;

    printf("supervisor: worker exited cleanly\n");
    fflush(stdout);
    return 0;
}

int main(void) {
    /*
     * Block SIGUSR1, SIGUSR2, and SIGCHLD BEFORE fork() so that signals sent
     * by the child cannot arrive in the parent before sigwait() is called.
     * Both processes inherit this mask across fork().
     */
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGUSR2);
    sigaddset(&block_mask, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        return 1;
    }

    if (child_pid == 0) {
        return run_worker(getppid());
    }

    return run_supervisor(child_pid);
}
