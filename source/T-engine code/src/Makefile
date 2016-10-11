#
#	@(#)Makefile
#		for GNU make
#
#	T-Kernel based sample program
#	Copyright (C) 2002-2003 by Personal Media Corporation
#
#	how to make
#	- make
#		make official version for release
#	- make mode=debug
#		create for debug
#	- make clean
#		delete all files created by make
#

# source dependency file (automatically created)
DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

# standard rule
include ../../etc/makerules

# ----------------------------------------------------------------------------

# target to be created
TARGET = drawsamp

# search path for source files
S = ../src
VPATH = $S

# source file
SRC = main.c drawsamp.c mem.c graphic.c bmp.c basic_class.c character_recognition.c feature_extraction.c pre_processing.c word_recognition.c

OBJ = $(addsuffix .o, $(basename $(SRC)))

# option
CFLAGS += $(CFLAGS_WARNING)

# ----------------------------------------------------------------------------

.PHONY: all install clean

ALL = $(TARGET) $(TARGET:%=%.map)

all: $(ALL)

$(TARGET): $(OBJ)
	$(LINK.o) $(LDOBJS) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJ) $(ALL) $(DEPS)

install: $(addprefix $(EXE_INSTALLDIR)/, $(ALL))

$(TARGET:%=$(EXE_INSTALLDIR)/%): $(EXE_INSTALLDIR)/%: %
	$(EXE_INSTALL_STRIP)


# dependency of source files
