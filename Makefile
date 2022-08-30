
OBJ_DIR		:= obj
SRC_DIR		:= src

CC			:= cc
CARGS		:= -Wall -Wextra -Werror -pedantic -ggdb
LIBS		:= -lSDL2

OUTPUT_DIR	:= .
OUTPUT		:= run

objects		+= main.o

build: $(addprefix $(OBJ_DIR)/, $(objects))
	$(CC) $(CARGS) -o $(OUTPUT_DIR)/$(OUTPUT) $^ $(LIBS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) -c $(CARGS) -o $@ $^

run:build
	$(OUTPUT_DIR)/$(OUTPUT)

gdb:build
	gdb $(OUTPUT_DIR)/$(OUTPUT)	
