#
# Created by gmakemake (Ubuntu Apr 19 2024) on Wed Oct 22 21:44:39 2025
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS =	-std=c99 -ggdb -Wall -Wextra -pedantic
CLIBFLAGS =	-lm 


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	args.c display.c getopt_demo.c simulation.c wildfire.c
PS_FILES =	
S_FILES =	
H_FILES =	args.h display.h simulation.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	args.o display.o simulation.o 

#
# Main targets
#

all:	getopt_demo wildfire 

getopt_demo:	getopt_demo.o $(OBJFILES)
	$(CC) $(CFLAGS) -o getopt_demo getopt_demo.o $(OBJFILES) $(CLIBFLAGS)

wildfire:	wildfire.o $(OBJFILES)
	$(CC) $(CFLAGS) -o wildfire wildfire.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

args.o:	args.h simulation.h
display.o:	display.h
getopt_demo.o:	
simulation.o:	display.h simulation.h
wildfire.o:	args.h simulation.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) getopt_demo.o wildfire.o core

realclean:        clean
	-/bin/rm -f getopt_demo wildfire 
