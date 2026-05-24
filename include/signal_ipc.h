#ifndef SIGNAL_IPC_H
#define SIGNAL_IPC_H

#include <sys/types.h>

/* Number of tasks the supervisor sends to the worker. */
#define NUM_TASKS 3

/*
 * run_worker - child process entry point.
 * supervisor_pid: PID of the parent/supervisor process.
 * Returns 0 on success, 1 on error.
 */
int run_worker(pid_t supervisor_pid);

/*
 * run_supervisor - parent process entry point.
 * worker_pid: PID of the child/worker process.
 * Returns 0 on success, 1 on error.
 */
int run_supervisor(pid_t worker_pid);

#endif /* SIGNAL_IPC_H */
