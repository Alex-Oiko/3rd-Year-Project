# makefile based on the Heat2D SpiNNaker example

PROG		= spintest
THUMB   	= no
CFLAGS  	= -DDEBUG
LIB_DIR 	= ../../sark/lib
INC_DIR 	= ../../sark/inc
FPMATH		= ../../include
INT_DIR		= build
OUT_DIR		= output
MAIN_SRC	= ../../testing/$(PROG).cpp

CC := armcc -c --cpu=5te --cpp --apcs /interwork -I$(INC_DIR) -I$(INT_DIR) -I$(FPMATH) $(CFLAGS)
CT := $(CC)
AS := armasm --keep --cpu=5te --apcs /interwork
RM := rm -f

ifeq ($(THUMB),yes)
  CT += --thumb -DTHUMB
  AS += --pd "THUMB seta 1" 
endif


OBJS = $(INT_DIR)/$(PROG).o


target: $(OBJS) example.sct $(INT_DIR) $(OUT_DIR)
	armlink --scatter=example.sct --output $(INT_DIR)/$(PROG).elf $(LIB_DIR)/spin1_api_lib.o $(OBJS)
	fromelf  $(INT_DIR)/$(PROG).elf --bin --output $(OUT_DIR)/$(PROG).aplx
	fromelf  $(INT_DIR)/$(PROG).elf -cds --output $(OUT_DIR)/$(PROG).lst

$(INT_DIR)/$(PROG).o: $(MAIN_SRC) $(FPMATH)/fpmath/fixed32.hpp $(INT_DIR) $(OUT_DIR)
	$(CT) $(MAIN_SRC) -o $@

$(INT_DIR):
	mkdir $(INT_DIR)

$(OUT_DIR):
	mkdir $(OUT_DIR)

clean:
	$(RM) $(INT_DIR)/*.o $(OUT_DIR)/*.aplx $(INT_DIR)/*.elf $(OUT_DIR)/*.lst *~
	${RM} APLX.bin RO_DATA.bin RW_DATA.bin a.out intdefs.h intalias.h

tidy:
	$(RM) $(INT_DIR)/*.o $(INT_DIR)/*.elf $(OUT_DIR)/*.lst *~
	${RM} APLX.bin RO_DATA.bin RW_DATA.bin a.out intdefs.h intalias.h
