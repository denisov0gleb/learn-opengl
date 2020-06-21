CC = gcc
CFLAGS = -o

EXE_DIR = build
EXE = main.exe
FILE = src/main.c

LIBS = -lglfw3 -lopengl32 -lgdi32

FLAGS = -std=c99

LIB_DIR = -I"./libs/glfw/include/" -I"./libs/gl3w/GL" -L"./libs/glfw/" -L"./libs/gl3w/GL"

ifeq ($(OS),Windows_NT) 
	REMOVE = del /s /q
	REMOVE_DIR = rmdir /s /q
else
	REMOVE = rm -rf
endif


all: $(EXE)

$(EXE): | $(EXE_DIR)
	$(CC) $(CFLAGS) $(EXE_DIR)/$(EXE) $(FILE) $(LIBS) $(LIB_DIR) $(FLAGS) 

$(EXE_DIR):
	mkdir $@

exe:
	./$(EXE_DIR)/$(EXE)

clear:
	$(REMOVE) *.exe *.o
	$(REMOVE_DIR) $(EXE_DIR)
