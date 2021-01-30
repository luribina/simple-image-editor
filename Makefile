CC = gcc
CFLAGS = -std=c18 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG
LDFLAGS = -lm

BUILD_DIR = ./build
SRC_DIR = ./src
FORMAT_DIR:= $(SRC_DIR)/formats
ACTION_DIR:= $(SRC_DIR)/actions

TARGET = image_editor

SOURCES = $(shell find $(SRC_DIR) -name *.c)
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: dir $(BUILD_DIR)/$(TARGET)

dir:
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR) $(BUILD_DIR)/$(FORMAT_DIR) $(BUILD_DIR)/$(ACTION_DIR)

$(OBJECTS): $(BUILD_DIR)/%.o : %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -r $(BUILD_DIR)