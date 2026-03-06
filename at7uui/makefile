#-------------------------#
# BEGIN CONFIG. VARS
#-------------------------#

C_FLAGS = -Wall -Wextra -Werror -g -std=c99
BLD_DIR = ../bin

#-------------------------#
# END CONFIG. VARS
#-------------------------#


#-------------------------#
# BEGIN COMP. VARS
#-------------------------#

UUI_A = ${UUI_DIR}/bin/uui.a
INC = -I${UUI_DIR}/uui/include -I${TWIS_DIR}/TWIS/include -I.


#-------------------------#
# END COMP. VARS
#-------------------------#

$(BLD_DIR)/at7.o: $(wildcard at7/*) $(UUI_A)
	gcc $(C_FLAGS) $(INC) at7uui.c -c -o $@
