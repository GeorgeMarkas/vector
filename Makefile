SRC_DIR   := .
TEST_DIR  := test
BUILD_DIR := build

SRC  := $(wildcard $(SRC_DIR)/*.c)
INC  := $(addprefix -I, .)
TEST := $(wildcard $(TEST_DIR)/*.c)
OBJ  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC)) \
	    $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST))

TARGET = $(BUILD_DIR)/run_tests

CC         := gcc
CFLAGS 	   := -Wall -Wextra -Werror -Wcast-qual -pedantic -std=c99
ASAN_FLAGS := -fsanitize=address,undefined

vpath %.c $(SRC_DIR) $(TEST_DIR)

all: $(TARGET)

.PHONY: all clean test

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $^ -o $@

test: $(TARGET)
	./$<

clean:
	rm -rf $(BUILD_DIR)