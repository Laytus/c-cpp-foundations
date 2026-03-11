CC := clang

CSTD := -std=c17
WARN := -Wall -Wextra -Werror
DBG := -O0 -g
SAN := -fsanitize=address,undefined
INC := -Iinclude

CFLAGS := $(CSTD) $(WARN) $(DBG) $(SAN) $(INC)
LDFLAGS := $(SAN)
LDLIBS :=

BUILD_DIR := build

MAIN ?= main
APP := $(BUILD_DIR)/csvstat
MAIN_SRC := app/$(MAIN).c
MAIN_OBJ := $(BUILD_DIR)/$(MAIN).o

SRCS := \
	src/line_reader.c \
	src/csv.c \
	src/stats.c \
	src/csvstat_err.c \
	$(MAIN_SRC)

OBJS := \
	$(BUILD_DIR)/line_reader.o \
	$(BUILD_DIR)/csv.o \
	$(BUILD_DIR)/stats.o \
	$(BUILD_DIR)/csvstat_err.o \
	$(MAIN_OBJ)

.PHONY: all run clean rebuild test help

all: $(APP)


$(APP): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/line_reader.o: src/line_reader.c include/line_reader.h include/csvstat_assert.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/csv.o: src/csv.c include/csv.h include/csvstat_assert.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stats.o: src/stats.c include/stats.h include/csvstat_assert.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/csvstat_err.o: src/csvstat_err.c include/csvstat_err.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC) include/line_reader.h include/csv.h include/stats.h include/csvstat_err.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(APP)
	./$(APP) tests/input/basic.csv price

test: $(APP)
	@echo "==> basic (positional)"
	./$(APP) tests/input/basic.csv price

	@echo "==> invalid numerics (mixed positional + --quiet)"
	./$(APP) tests/input/invalid.csv price --quiet

	@echo "==> nan/inf/overflow (flag mode)"
	./$(APP) --file tests/input/nan_inf.csv --col price --quiet

	@echo "==> one valid sample"
	./$(APP) --file tests/input/one_ok.csv --col price --quiet

	@echo "==> zero valid samples"
	./$(APP) --file tests/input/zero_ok.csv --col price --quiet

	@echo "==> header only"
	./$(APP) tests/input/header_only.csv price --quiet

	@echo "==> missing cells / short row"
	./$(APP) tests/input/missing_cells.csv price --quiet

	@echo "==> spaces trimming"
	./$(APP) tests/input/spaces.csv price --quiet

	@echo "==> empty file should fail with format error"
	! ./$(APP) tests/input/empty.csv price

# "!" tells the shell this command is expected to fail.

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

help:
	@echo "Targets:"
	@echo "  make         Build the project"
	@echo "  make run     Build and run a basic example"
	@echo "  make test    Build and run core test commands"
	@echo "  make clean   Remove build artifacts"
	@echo "  make rebuild Clean and rebuild"