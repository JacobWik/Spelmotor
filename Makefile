
SRS_DIR = src
BUILD_DIR = build/debug
CC = g++
SRS_FILES = $(wildcard $(SRS_DIR)/*.cpp)
OBJ_NAME = play
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g

all:
	$(CC) $(COMPILER_FLAGS) $(SRS_FILES) -o $(BUILD_DIR)/$(OBJ_NAME) -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image