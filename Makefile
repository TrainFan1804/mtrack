# compiler and linker flags
CXX			:= 	g++
CXXFLAGS	:=	-g -O0 -Wall
LDFLAGS		:=	-lsqlite3 -lQt5Widgets -lQt5Core -lQt5Gui

# directories
TARGET_EXEC	:=	mtrack
BUILD_DIR	:=	./build
SRC_DIR		:= 	./src
INC_PATH	:= 	./include

# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(INC_PATH) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -I/usr/include/qt -I/usr/include/qt/QtWidgets

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP -DDEBUG
# VERSION	:= "1.3.2"
VERSION := $(shell git rev-parse --abbrev-ref HEAD)
CPPFLAGS += -DVERSION=\"$(VERSION)\"

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Successfully build application"

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@echo "Compiling $< -> $@"

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	@echo "Compiling $< -> $@"

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
