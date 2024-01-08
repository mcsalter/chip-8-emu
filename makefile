# Target executable
completed_file	:= chip8.out
lib		:= chip8.a
libobjs	:= chip8.o
objs		:= main.o
outs		:= $(completed_file) $(lib)


# Compiler Args:
CC		:= gcc
AR		:= ar
ARFLAGS	:= rcs
CCFLAGS	+= -Wall -std=c2x -fstack-protector -pipe
CCLINKLIB	:= $(lib)


# Variables:

ifneq ($(V),1) #verbose off by default
Q = @
endif


CCFLAGS_DEBUG += -ggdb -pedantic -Werror
CCFLAGS_REL += -O2

#debug phony
.PHONY: debug
debug: Q=
debug: CCFLAGS += $(CCFLAGS_DEBUG)
debug: OUTPUT    =	@echo -e " [OK]   Compiling: " $@
debug: AROUTPUT    =	@echo -e " [OK]   Archiving: " $@
debug: CLEANOUT  =	@echo -e " [OK]   CLEAN"
debug: CDEPOUT   =	@echo -e " [OK]   CLEAN DEPS"
debug: $(outs)

#release phony
.PHONY: release
release: OUTPUT    =	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m"
release: OUTPUT   +=		 "Compiling:\033[36;13m" $@ "\033[m"
release: AROUTPUT    =	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m"
release: AROUTPUT   +=		 "Archiving:\033[36;13m" $@ "\033[m"
release: CLEANOUT  =	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m CLEAN \033[36;13m \033[m"
release: CDEPOUT   =	@echo -e "\033[0;32m [OK] \033[0m   \033[0;33m CLEAN DEPS \033[36;13m \033[m"
release: CCFLAGS += $(CCFLAGS_REL)
release: $(outs)


# Make Commands:
all: $(outs) #clean-deps

# Dependencies:

deps	:= $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(completed_file): $(objs) $(lib)
	$(OUTPUT)
	$(Q) $(CC) $(CCFLAGS) -o $@ $^ $(CCLINKLIB)

$(lib): $(libobjs)
	$(AROUTPUT)
	$(Q) $(AR) $(ARFLAGS) $@ $<

%.o: %.c
	$(OUTPUT)
	$(Q) $(CC) $(CCFLAGS) -c -o $@ $< $(DEPFLAGS)

.PHONY: clean
clean:
	$(CLEANOUT)
	rm -f $(outs) $(objs) $(deps) $(libobjs)

.PHONY: clean-deps
clean-deps:
	$(CDEPOUT)
	$(Q) rm -f $(objs) $(deps) $(libojs)
