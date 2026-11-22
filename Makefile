SRC_DIR   := .
TEST_DIR  := test
BUILD_DIR := build

TARGET := $(BUILD_DIR)/libvector.a

SRC := $(wildcard $(SRC_DIR)/*.c)
INC := $(addprefix -I, $(SRC_DIR))
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

TEST_TARGET := $(BUILD_DIR)/tests

TEST 	 := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST))

CFLAGS 	  := -Wall -Wextra -Wcast-qual -Wconversion -Wsign-conversion -Wshadow -std=c99 -pedantic
SAN_FLAGS := -fsanitize=address,undefined

RELEASE_FLAGS := $(CFLAGS) -Werror -O2
DEBUG_FLAGS   := $(CFLAGS) $(SAN_FLAGS) -g
TEST_FLAGS    := $(filter-out -pedantic, $(CFLAGS)) $(SAN_FLAGS) -Wno-unused-parameter -g

FLAGS := $(RELEASE_FLAGS)

.PHONY: release debug test clean

release: $(TARGET)

debug: FLAGS := $(DEBUG_FLAGS)
debug: $(TARGET)

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	ar rcs $@ $^

test: FLAGS := $(DEBUG_FLAGS)
test: $(TEST_TARGET)

$(TEST_OBJ): $(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(TEST_FLAGS) $(INC) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJ) $(OBJ)
	@$(CC) $(SAN_FLAGS) $^ -o $@
	@./$@

clean:
	rm -rf $(BUILD_DIR)