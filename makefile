# Target library
emu		:= chip-8.out
objs		:= main.o chip8.o gui.o

# Compiler Args:
CC		:= gcc
CCFLAGS		:= -Wall -Werror -std=c2x -fstack-protector
LINKERFLAGS 	:= -lncurses -ltinfo #-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf


# Variables:

ifneq ($(V),1) #verbose off by default
Q = @
endif

ifeq ($(D),1) # debug off by default
CCFLAGS += -ggdb
else
CCFLAGS += -O2
endif

ifeq ($(F),1) #defaults to plain formatting
OUTPUT	= 	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m"
OUTPUT	+=             "Compiling:\033[36;13m" $< "\033[m"
else
OUTPUT	= 	@echo -e " [OK]   Compiling: " $<
endif

# Make Commands:
all: $(emu) clean-deps

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

.PHONY: clean-deps
clean-deps:
	@echo -e  "\033[0;32m [OK] \033[0m   \033[0;33m CLEAN DEPS \033[36;13m \033[m"
	$(Q) rm -f $(objs) $(deps)
