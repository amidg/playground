# Compiler
CXX := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Directories
SRCDIR := src
BUILDDIR := build
MODULES := tree

# Files
SRCFILES := $(wildcard $(SRCDIR)/**/*.cpp) $(wildcard $(SRCDIR)/*.cpp)
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCFILES))

# Main target
TARGET := main
.PHONY: all clean
all: $(TARGET)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Linking
$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILDDIR)/$@

# Compilation
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	#$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(BUILDDIR)


