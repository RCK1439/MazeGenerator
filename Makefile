CXX = g++
CXXFLAGS_DEBUG = -Wall -g -DMG_DEBUG
CXXFLAGS_RELEASE = -O3 -DMG_RELEASE
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCDIR = MazeGenerator/src
BINDIR = bin
INTDIR = bin-int
DEBUG_FOLDER = Debug-linux-x64_x86
RELEASE_FOLDER = Release-linux-x64_x86

BIN_DEBUG = $(BINDIR)/$(DEBUG_FOLDER)
BIN_RELEASE = $(BINDIR)/$(RELEASE_FOLDER)

INT_DEBUG = $(INTDIR)/$(DEBUG_FOLDER)
INT_RELEASE = $(INTDIR)/$(RELEASE_FOLDER)

SOURCES := $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS_DEBUG := $(patsubst $(SRCDIR)/%.cpp,$(INT_DEBUG)/%.o,$(SOURCES))
OBJECTS_RELEASE := $(patsubst $(SRCDIR)/%.cpp,$(INT_RELEASE)/%.o,$(SOURCES))

TARGET_DEBUG = $(BIN_DEBUG)/MazeGenerator
TARGET_RELEASE = $(BIN_RELEASE)/MazeGenerator

.PHONY: all debug release clean

all: debug release

debug: $(INT_DEBUG) $(TARGET_DEBUG)

release: $(INT_RELEASE) $(TARGET_RELEASE)

$(INT_DEBUG):
	mkdir -p $(INT_DEBUG)
	mkdir -p $(BIN_DEBUG)

$(INT_RELEASE):
	mkdir -p $(INT_RELEASE)
	mkdir -p $(BIN_RELEASE)

$(TARGET_DEBUG): $(INT_DEBUG) $(OBJECTS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ $(OBJECTS_DEBUG) $(LIBS)

$(TARGET_RELEASE): $(INT_RELEASE) $(OBJECTS_RELEASE)
	$(CXX) $(CXXFLAGS_RELEASE) -o $@ $(OBJECTS_RELEASE) $(LIBS)

$(INT_DEBUG)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c -o $@ -I $(SRCDIR) $<

$(INT_RELEASE)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c -o $@ -I $(SRCDIR) $<

clean:
	rm -f $(OBJECTS_DEBUG) $(OBJECTS_RELEASE) $(TARGET_DEBUG) $(TARGET_RELEASE)
	rm -rf $(INTDIR)
	rm -rf $(BINDIR)

.PRECIOUS: $(OBJECTS_DEBUG) $(OBJECTS_RELEASE)