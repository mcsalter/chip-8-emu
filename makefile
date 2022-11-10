# Target library
emu	:= chip-8.out
objs	:= main.o chip8.o
objs_future := main.o chip8.o gui.o

# Compiler Args:
CC	:= gcc
CCFLAGS	:= -Wall -Werror -std=c2x
LINKERFLAGS := #-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
AR	:= ar
ARFLAGS	:= rcs

# Variables:

ifneq ($(V),1) #verbose off by default
Q = @
endif

ifeq ($(D),1) # debug off by default
CCFLAGS += -g
else
CCFLAGS += -O2
endif

OUTPUT	= 	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m"
OUTPUT	+=             "Compiling:\033[36;13m" $< "\033[m"


# Make Commands:
all: $(emu)

# Dependencies:

deps	:= $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS	= -MMD -MF $(@:.o=.d)

$(emu): $(objs)
	$(OUTPUT)
	$(Q) $(CC) $(CCFLAGS) $(LINKERFLAGS)  -o $@ $^

%.o: %.c
	$(OUTPUT)
	$(Q) $(CC) $(CCFLAGS) -c -o $@ $< $(DEPFLAGS)

.PHONY: clean
clean:
	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m CLEAN \033[36;13m \033[m"
	$(Q) rm -f $(emu) $(objs) $(deps)
