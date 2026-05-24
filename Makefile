CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -Iinclude
BIN_DIR = bin

all: $(BIN_DIR)/signal_ipc

solution: $(BIN_DIR)/signal_ipc_solution

check: all
	./scripts/check.sh

grade: all
	./scripts/grade.sh

$(BIN_DIR)/signal_ipc: src/signal_ipc.c include/signal_ipc.h
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/signal_ipc_solution: solutions/signal_ipc_solution.c include/signal_ipc.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_DIR)/signal_ipc $(BIN_DIR)/signal_ipc_solution

.PHONY: all solution check grade clean
