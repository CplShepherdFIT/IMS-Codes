# Generic Makefile for building C programs (Linux/WSL-friendly)
# - Builds a bare executable for each .c file in the workspace root
# - Compiles and links against sources in lib/
# - Supports dependency tracking and common helpers

SHELL := /bin/sh

# Toolchain configuration (override on command line if desired)
# CSTD chooses the ISO C standard level (c89, c99, c11, c17)
CSTD ?= c17
CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=$(CSTD) -pedantic
CPPFLAGS ?= -Ilib
LDFLAGS ?=

# Optional executable extension (empty on Linux/WSL; set to .exe on Windows if needed)
EXT ?=

# Source discovery
SRC := $(wildcard *.c)
PROGS := $(basename $(SRC))
TARGETS := $(addsuffix $(EXT),$(PROGS))
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

# Library sources and objects
LIB_SRC := $(wildcard lib/*.c)
LIB_OBJ := $(LIB_SRC:.c=.o)

.PHONY: all clean run help list

# Default: build all executables in the root folder
all: $(TARGETS)

# Compile root sources to objects with dependency files
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

# Compile library sources to objects with dependency files (objects reside in lib/)
lib/%.o: lib/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

# Link each root program with library objects.
# Use a *static pattern rule* so GNU Make treats this as an explicit rule for
# our discovered targets (otherwise its built-in implicit rule may link directly
# from %.c and skip $(LIB_OBJ)).
$(TARGETS): %$(EXT): %.o $(LIB_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Auto-include generated dependency files (safe if they don't exist yet)
-include $(DEP)
-include $(LIB_SRC:.c=.d)

# Convenience helpers
help:
	@echo "Targets:"
	@echo "  make                Build all executables for *.c in root"
	@echo "  make <name>         Build a single program (from <name>.c)"
	@echo "  make run FILE=...   Run an executable (ARGS=... optional)"
	@echo "  make list           Show discovered sources and outputs"
	@echo "  make clean          Remove build artifacts"
	@echo "Variables (override as needed):"
	@echo "  CC=<compiler>       Default: gcc"
	@echo "  CSTD=<c89|c99|c11|c17>  Default: c17 (ISO C)"
	@echo "  CFLAGS=...          Default: -O2 -Wall -Wextra -std=$(CSTD) -pedantic"
	@echo "  EXT=                 Executable suffix (empty for Linux, .exe for Windows)"

list:
	@echo "Source files: $(SRC)"
	@echo "Library files: $(LIB_SRC)"
	@echo "Executables: $(TARGETS)"

# Run helper: FILE=<program.exe> [ARGS=...] (uses current directory)
run:
ifndef FILE
	@echo "Usage: make run FILE=<program$(EXT)> [ARGS=...]"; exit 1
else
	@echo "Running $(FILE) $(ARGS)..."
	./$(FILE) $(ARGS)
endif

# Clean up build artifacts (GNU Make provides $(RM) = rm -f)
clean:
	$(RM) $(OBJ) $(DEP) $(TARGETS) $(LIB_OBJ) $(LIB_SRC:.c=.d)

# Convenience: allow `make Euler` to build `Euler$(EXT)`.
# IMPORTANT: only enable this when EXT is non-empty, otherwise the rule
# becomes `%: %` and GNU Make reports circular dependencies.
ifneq ($(strip $(EXT)),)
%: %$(EXT)
	@true
endif
