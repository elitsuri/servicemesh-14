CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lsqlite3 -lpthread
TARGET  = servicemesh
SRCDIR  = src
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
