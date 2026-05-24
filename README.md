# System Programming Lab: Signal-Based IPC — Supervisor/Worker

## 1. Learning Objectives
By the end of this lab you should be able to:
- use `fork()` to create a worker process and coordinate it with signals
- block signals with `sigprocmask()` before `fork()` to prevent race conditions
- send signals between processes with `kill()`
- use `sigwait()` to receive blocked signals atomically and without busy-waiting
- implement a clean shutdown sequence using `SIGUSR1`, `SIGUSR2`, and `SIGCHLD`
- reap a child process with `waitpid()`

## 2. Background
Signals are a fundamental POSIX inter-process communication mechanism. A common
pattern is to **block** a set of signals with `sigprocmask()` before `fork()` and
then receive them synchronously with `sigwait()` in both processes. This avoids the
classic race condition where a signal can arrive between an `if(!flag)` check and a
`pause()` call, causing the program to hang forever.

In this lab you will build a **supervisor/worker** system:

```
supervisor (parent)                worker (child)
─────────────────                  ──────────────
waits for "ready"  <─── SIGUSR1 ── prints "worker: ready"
prints "sending task 1"
sends SIGUSR1  ────────────────>   prints "worker: received task 1"
                   <─── SIGUSR1 ── sends ack
      ...                              ...
prints "shutting down"
sends SIGUSR2  ────────────────>   prints "worker: all tasks done, exiting"
waits SIGCHLD  <─── SIGCHLD ─────  (process exits)
prints "worker exited cleanly"
```

## 3. Repository Layout
```
src/         student source file (fill in the TODOs)
include/     header with function prototypes and NUM_TASKS
solutions/   instructor reference implementation
scripts/     local test, check, and grade helpers
tests/       notes on the visible checks
bin/         compiled binaries (git-ignored)
```

## 4. What You Need To Implement

Open `src/signal_ipc.c`. Complete every `TODO` comment.

### `run_worker(pid_t supervisor_pid)`
1. Build a `sigset_t` containing `SIGUSR1` and `SIGUSR2`.
2. Print `worker: ready` and flush stdout.
3. Send `SIGUSR1` to `supervisor_pid` (readiness notification).
4. Loop `NUM_TASKS` times: call `sigwait()` for the next signal.  
   On `SIGUSR1`: print `worker: received task N`, flush, send `SIGUSR1` ack.
5. Call `sigwait()` once more for `SIGUSR2` (shutdown).
6. Print `worker: all tasks done, exiting` and flush.

### `run_supervisor(pid_t worker_pid)`
1. Build a `sigset_t` containing `SIGUSR1` and `SIGCHLD`.
2. Call `sigwait()` to wait for the worker's ready `SIGUSR1`.
3. For each task `i = 1 .. NUM_TASKS`:
   - Print `supervisor: sending task i` and flush.
   - Send `SIGUSR1` to `worker_pid`.
   - Call `sigwait()` to wait for the ack `SIGUSR1`.
4. Print `supervisor: shutting down worker` and flush.
5. Send `SIGUSR2` to `worker_pid`.
6. Call `sigwait()` for `SIGCHLD` (worker exit notification).
7. Call `waitpid()` to reap the worker.
8. Print `supervisor: worker exited cleanly` and flush.

### Rules
- do **not** change `main()` or the function signatures in `include/signal_ipc.h`
- use `sigwait()` (not `pause()` or busy-loops) for waiting
- check all return values of `kill()`, `sigwait()`, and `waitpid()`
- do **not** call `printf` or `fflush` from inside a signal handler

## 5. Expected Output

Running `./bin/signal_ipc` must print **exactly**:

```
worker: ready
supervisor: sending task 1
worker: received task 1
supervisor: sending task 2
worker: received task 2
supervisor: sending task 3
worker: received task 3
supervisor: shutting down worker
worker: all tasks done, exiting
supervisor: worker exited cleanly
```

## 6. Build
```bash
make
```

## 7. Run
```bash
./bin/signal_ipc
```

## 8. Test
```bash
./scripts/test.sh
```

## 9. Grading Hooks
```bash
make check   # same as test.sh
make grade   # test.sh + grade banner
```

## 10. Grading Rubric
| Criterion | Weight |
|---|---|
| Correct output (all 10 lines, exact text) | 50% |
| `sigwait()` used for both processes | 20% |
| `kill()` called for each direction | 15% |
| `waitpid()` used to reap child | 10% |
| Builds with no warnings | 5% |

## 11. Submission Checklist
- [ ] builds with `make` and no warnings
- [ ] `./scripts/test.sh` exits 0
- [ ] all `TODO` comments replaced with working code

## 12. Academic Integrity
Write your own solution. Discussion of concepts is allowed; sharing code is not.

## 13. Instructor Materials
Instructor solution: `solutions/signal_ipc_solution.c`  
Remove the `solutions/` directory before publishing as a Classroom template.
