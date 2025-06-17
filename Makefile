# ATM Management System Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS = 

# Directories
SRCDIR = src
DATADIR = data
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/auth.c $(SRCDIR)/system.c $(SRCDIR)/secondary.c $(SRCDIR)/ui.c $(SRCDIR)/error_handler.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HEADERS = $(SRCDIR)/header.h

# Target executable
TARGET = atm_system

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Create data directory if it doesn't exist
$(DATADIR):
	mkdir -p $(DATADIR)
	touch $(DATADIR)/users.txt
	touch $(DATADIR)/records.txt

# Build target
$(TARGET): $(OBJDIR) $(DATADIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	@echo "Clean complete!"

# Clean everything including data
clean-all: clean
	rm -rf $(DATADIR)
	@echo "All files cleaned!"

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/"

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled from /usr/local/bin/"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Check for memory leaks (requires valgrind)
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Format code (requires clang-format)
format:
	clang-format -i $(SOURCES) $(HEADERS)

# Static analysis (requires cppcheck)
analyze:
	cppcheck --enable=all --std=c99 $(SOURCES)

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the program (default)"
	@echo "  clean     - Remove build files"
	@echo "  clean-all - Remove build files and data"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  run       - Build and run the program"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build optimized release version"
	@echo "  memcheck  - Run with valgrind memory checker"
	@echo "  format    - Format source code"
	@echo "  analyze   - Run static analysis"
	@echo "  help      - Show this help message"

# Phony targets
.PHONY: all clean clean-all install uninstall run debug release memcheck format analyze help