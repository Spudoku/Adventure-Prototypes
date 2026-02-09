#Adapted from 
#https://github.com/AtariLynx/programming-tutorial/blob/main/helloworld/Makefile


#these may need to detect windows or linux and adjust accordingly
ifeq ($(CC65_INC),)
	CC65_INC=/usr/share/cc65/include
endif
ifeq ($(CA65_INC),)
	CA65_INC=/usr/share/cc65/asminc
endif
ifeq ($(BUILD),)
	BUILD=debug
endif

SYS=atari

CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
CP=cp
RM=rm -f
ECHO=echo
TOUCH=touch



# Flag for assembler
AFLAGS=-I $(CA65_INC) --debug-info -t $(SYS) -D $(BUILD)

# Flags for C-code compiler
CFLAGS=-I ./includes -t $(SYS) --add-source -O -g -Or -Cl -Wl --dbgfile,test.dbg -C atari_modifed.cfg -Os -D $(BUILD)

target = test.xex
viceLabel = game.lbl
objects = player.o test.o
		  
$(target) : $(objects)
	$(CL) -t $(SYS) -o $@ $(objects) 

# Rule for making a *.o file out of a *.c file
%.o: %.c
	$(CC) $(CFLAGS) -o $(*).s $<
	$(AS) -o $@ $(AFLAGS) $(*).s
	$(RM) $*.s

# Rule for making a *.o file out of a *.s or *.asm file
%.o: %.s
	$(AS) -o $@ $(AFLAGS) $<
%.o: %.asm
	$(AS) -o $@ $(AFLAGS) $<

all: $(target)

clean :
	$(RM) $(objects)
	$(RM) *.spr
	$(RM) *.s
	$(RM) *.pal