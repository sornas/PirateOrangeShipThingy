GAME = pirates
FOG_DIR = fog
LIB_DIR = lib
INC_DIR = inc
BIN_DIR = bin

ARCH := $(shell uname -s | cut -c -5)

CC = gcc
CXX = g++

STD = c++17
WARNINGS = -Wall -Wno-missing-braces
FLAGS = $(WARNINGS) -std=$(STD)
DEBUG_FLAGS = $(FLAGS) -ggdb -O0
LIBS = -lfog -lSDL2 -lSDL2main -lpthread

ifeq ($(TARGET),WINDOWS)
	ARCH = MINGW
	CC = x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++
	LIBS += -static-libgcc -static-libstdc++
endif

ENGINE = libfog.a
ifeq ($(ARCH),MINGW)
	# Cross compilation
	ENGINE = libfog.lib
endif
ENGINE_PATH = $(LIB_DIR)/$(ENGINE)

ifneq ($(ARCH),MINGW)
	LIBS += -ldl -lm
endif
ifeq ($(ARCH),MINGW)
	LIBS += -L/mingw64/lib
endif
INCLUDES = -I$(INC_DIR)

ASSET_BUILDER = $(FOG_DIR)/out/mist
ASSET_FILE = data.fog
ASSETS := $(shell find res -type f -name "*.*")

HEADERS := $(shell find src -type f -name "*.h")
SOURCES := $(shell find src -type f -name "*.cpp")
OBJECTS := $(SOURCES:src/%.cpp=$(BIN_DIR)/%.o)

default: all
all: game

game: $(GAME)
$(GAME): $(ENGINE_PATH) $(OBJECTS) $(ASSET_FILE)
	$(CXX) $(DEBUG_FLAGS) $(OBJECTS) -o $@ -L$(LIB_DIR) $(LIBS)

.PHONY: run
run: $(GAME)
	./$(GAME)

.PHONY: debug
debug: $(GAME)
	gdb -ex "b _fog_illegal_allocation" -ex "b _fog_close_app_responsibly" ./$(GAME)

$(BIN_DIR)/%.o: src/%.cpp $(HEADERS) | $(BIN_DIR)
	$(CXX) $(DEBUG_FLAGS) -c $< -o $@ $(INCLUDES)

.PHONY: engine $(ENGINE_PATH)
engine: $(ENGINE_PATH)
.NOTPARALLEL: $(ENGINE_PATH)
$(ENGINE_PATH): | $(LIB_DIR) $(INC_DIR)
	make -C $(FOG_DIR) engine ENGINE_LIBRARY_NAME=$(ENGINE) CXX=$(CXX)
	cp $(FOG_DIR)/out/$(ENGINE) $(LIB_DIR)/
	cp $(FOG_DIR)/out/fog.h $(INC_DIR)/

$(ASSET_BUILDER):
	make -C $(FOG_DIR) mist

.PHONY: assets
assets: $(ASSET_FILE)
$(ASSET_FILE): $(ASSETS) $(ASSET_BUILDER)
	$(ASSET_BUILDER) -o $@ $(ASSETS)

.PHONY: clean
clean:
	rm -f $(GAME)
	rm -f $(GAME).exe
	rm -f $(ASSET_FILE)
	rm -rf $(INC_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)

.PHONY: clean-all
clean-all: clean
	make -C fog clean

$(LIB_DIR):
	mkdir -p $@

$(INC_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@
