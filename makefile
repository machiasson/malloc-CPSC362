CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = tests

# Files
SRCS = tests.c malloc.c
HEADERS = malloc_header.h

# Create executable from source files
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean up generated files
clean:
	rm -f $(TARGET) *.o

# Run tests after building
run: $(TARGET)
	./$(TARGET)