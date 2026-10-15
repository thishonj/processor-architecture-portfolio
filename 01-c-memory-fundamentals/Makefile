CC      ?= gcc
# -Wno-format-truncation: 02_arrays_strings.c truncates a string on purpose to show snprintf is safe
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-format-truncation -O0 -g
SRC_DIR := src
BIN_DIR := build
PROGS   := 01_pointers 02_arrays_strings 03_structs_padding 04_memory_layout 06_ring_buffer
BINS    := $(addprefix $(BIN_DIR)/,$(PROGS))

.PHONY: all run asan clean
all: $(BINS) $(BIN_DIR)/05_debug_bugs

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	@for p in $(PROGS); do echo "=== $$p ==="; ./$(BIN_DIR)/$$p; echo; done

# Debug-bug program built with sanitizers: build/05_debug_bugs_asan
asan: | $(BIN_DIR)
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer \
	    $(SRC_DIR)/05_debug_bugs.c -o $(BIN_DIR)/05_debug_bugs_asan

clean:
	rm -rf $(BIN_DIR)
