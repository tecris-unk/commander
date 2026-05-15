CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS ?=
LDLIBS ?= -lncurses

SRC := main.c ui.c panel.c fs.c input.c
OBJ := $(SRC:.c=.o)
TARGET := fm

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)