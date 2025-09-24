###############################################################################
# CONFIGURATION
###############################################################################

# Project info
PROJECT_NAME := 3DViewer
VERSION := 1.0

# Compiler and flags
CXX		:= g++
CXXFLAGS := -std=c++20 -Wall -Wextra -fprofile-arcs -ftest-coverage
LDFLAGS := -lgtest -lpthread -lgcov

# Directories
SRC_DIR := .
MODEL_DIR := $(SRC_DIR)/model
VIEW_DIR := $(SRC_DIR)/view
CONTROLLER_DIR := $(SRC_DIR)/controller
TEST_DIR := $(SRC_DIR)/tests
DVI_DIR := $(SRC_DIR)/dvi
COV_DIR := $(SRC_DIR)/coverage
BUILD_DIR := $(SRC_DIR)/build
BUILD_TEST_DIR := $(SRC_DIR)/test_build
DIST_DIR := $(SRC_DIR)/$(PROJECT_NAME)_$(VERSION)

# Files
QT_PROJECT_FILE := $(VIEW_DIR)/untitled.pro
MAIN_APP := $(BUILD_DIR)/untitled
TEST_APP := $(BUILD_TEST_DIR)/tests
DVI_FILE := $(DVI_DIR)/$(PROJECT_NAME).info
DIST_ARCHIVE := $(DIST_DIR).tar.gz

###############################################################################
# MAIN TARGETS
###############################################################################

.PHONY: all install uninstall clean dvi dist tests gcov_report format format-check run

# Default target - build and run the application
all: run

# Install and build the application
install:
	@echo "Building $(PROJECT_NAME) v$(VERSION)..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && qmake ../$(QT_PROJECT_FILE)
	@$(MAKE) -C $(BUILD_DIR)
	@echo "Build completed successfully."

# Remove installed application
uninstall:
	@echo "Removing $(PROJECT_NAME)..."
	@rm -rf $(BUILD_DIR)
	@echo "Uninstall completed."

# Clean build artifacts and temporary files
clean:
	@echo "Cleaning project..."
	@find . \( -name "*.o" -o -name "*.gcno" -o -name "*.gcda" -o -name "*.info" \) -exec rm -f {} +
	@rm -f $(DIST_ARCHIVE)
	@rm -rf $(BUILD_DIR) $(BUILD_TEST_DIR) $(COV_DIR)
	@echo "Clean completed."

# Generate documentation
dvi:
	@echo "Generating documentation..."
	@cd $(DVI_DIR) && makeinfo 3DViewer.texi
	@info $(DVI_FILE)
	@echo "Documentation saved to $(DVI_DIR)."

# Create distribution package
dist: clean
	@echo "Creating distribution package..."
	@mkdir -p $(DIST_DIR)
	@cp -r Makefile $(CONTROLLER_DIR) $(MODEL_DIR) $(VIEW_DIR) $(TEST_DIR) $(DIST_DIR)
	@tar -czvf $(DIST_ARCHIVE) $(DIST_DIR)
	@rm -rf $(DIST_DIR)
	@echo "Distribution package created: $(DIST_ARCHIVE)"

# Run tests
tests:
	@echo "Running tests..."
	@mkdir -p $(BUILD_TEST_DIR)
	@$(CXX) $(CXXFLAGS) $(TEST_DIR)/*.cc $(MODEL_DIR)/*.cc $(LDFLAGS) -o $(TEST_APP)
	@./$(TEST_APP)

# Generate test coverage report
gcov_report: clean tests
	@echo "Generating coverage report..."
	@-./$(TEST_APP)
	@mkdir -p $(COV_DIR)
	@gcovr -r . \
		--exclude "tests/*" \
		--html --html-details -o $(COV_DIR)/index.html \
		--print-summary
	@echo "Coverage report generated in $(COV_DIR)/"

###############################################################################
# ADDITIONAL TARGETS
###############################################################################

# Format source code
format:
	@echo "Formatting source code..."
	@cp ../materials/linters/.clang-format .clang-format
	@find . -name "*.cc" -o -name "*.h" | xargs clang-format-18 -i --Werror
	@rm .clang-format
	@echo "Formatting completed."

# Check code formatting
format-check:
	@echo "Checking code formatting..."
	@cp ../materials/linters/.clang-format .clang-format
	@find . -name "*.cc" -o -name "*.h" | xargs clang-format-18 -n --Werror
	@rm .clang-format
	@echo "Format check completed."

# Run the application
run: install
	@echo "Starting $(PROJECT_NAME)..."
	@./$(MAIN_APP)

###############################################################################
# HELP TARGET
###############################################################################

# Show help information
help:
	@echo "$(PROJECT_NAME) v$(VERSION) - Makefile targets:"
	@echo ""
	@echo "  all        - Build and run the application (default)"
	@echo "  install    - Build the application"
	@echo "  uninstall  - Remove the application"
	@echo "  clean      - Remove all build artifacts"
	@echo "  dvi        - Generate documentation"
	@echo "  dist       - Create distribution package"
	@echo "  tests      - Run tests"
	@echo "  gcov_report - Generate test coverage report"
	@echo "  format     - Format source code"
	@echo "  format-check - Check code formatting"
	@echo "  run        - Run the application"
	@echo "  help       - Show this help message"