CC = gcc
CFLAGS = -o

EXE_DIR = build
EXE = main.exe
FILE = src/main.c

LIBS = -lopengl32 -lglfw3 -lglew32 -lgdi32

FLAGS = -std=c99

INCLUDE_DIR = -I"./libs/" -I"./libs/glfw/include/"
STATIC_LIB_DIR = -L"./libs/GLEW/" -L"./libs/glfw/"

ifeq ($(OS),Windows_NT)
	REMOVE = del /s /q
	REMOVE_DIR = rmdir /s /q
else
	REMOVE = rm -rf
endif


all: $(EXE)

$(EXE): | $(EXE_DIR)
	$(CC) $(CFLAGS) $(EXE_DIR)/$(EXE) $(FILE) $(LIBS) $(INCLUDE_DIR) $(STATIC_LIB_DIR) $(FLAGS)

$(EXE_DIR):
	mkdir $@

exe:
	./$(EXE_DIR)/$(EXE)

clear:
	$(REMOVE) *.exe *.o
	$(REMOVE_DIR) $(EXE_DIR)
