#-------------------------#
# BEGIN CONFIG. VARS
#-------------------------#

C_FLAGS = -Wall -Wextra -Werror -g -std=c99
OBJ_DIR = build

#-------------------------#
# END CONFIG. VARS
#-------------------------#

BLD_DIR = ./bin

#-------------------------#
# BEGIN COMP. VARS
#-------------------------#

UUI_A = ${UUI_DIR}/bin/uui.a

ifeq ($(OS),Windows_NT)
	INC = -I${UUI_DIR}/uui/include -I${TWIS_DIR}/TWIS/include -I. -I..
else
	INC = -I${UUI_DIR}/uui/include -I${TWIS_DIR}/TWIS/include -I. -I..
endif

ifeq ($(OS),Windows_NT)
	LNK = ${TWIS_DIR}/TWIS/build/twis.a $(UUI_A) -lgdi32 -lopengl32 ../bin/at7uui.o
else
	LNK = -lgl
endif

#-------------------------#
# END COMP. VARS
#-------------------------#

$(BLD_DIR)/helloworld.exe: $(wildcard helloworld/*) $(UUI_A)
	gcc $(C_FLAGS) $(INC) helloworld/helloworld.c $(LNK) -o $@
helloworld: $(BLD_DIR)/helloworld.exe