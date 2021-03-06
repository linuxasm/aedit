#
# usage:  make         - compile asmedit executable
#         make clean   - touch all source files
#         make install - install files
#         make release - create release file
#
#
# modify the following as needed
#
# select assembler, nasm, fasm, as, yasm
#
assembler := nasm
#
#                      
#

ifeq "$(LIBS)"  ""
LIBS = ./AsmLib/asmlib.a
endif

#LFLAGS =
LFLAGS = -m elf_i386 -static

# the following variables are probably ok
######################################################
home = $(HOME)
here = $(shell pwd)
version = $(shell cat VERSION)
usr=$(shell basename $(HOME) )

ifeq "$(assembler)" "nasm"
CC = nasm
DEBUG = -g
CFLAGS = -f elf32 -O99
endif

ifeq "$(assembler)" "as"
CC = as
DEBUG = -stabs
CFLAGS =
endif

ifeq "$(assembler)" "yasm"
CC = yasm
DEBUG = -g stabs
CFLAGS = -f elf32
endif


ifeq "$(assembler)" "fasm"
CC = fasm
DEBUG =
CFLAGS =
endif

.SUFFIXES : .o .asm

#dirs := ShowSysErr

srcs := $(foreach dir,$(dirs),$(wildcard $(dir)/*.asm))     

objs := $(foreach dir,$(dirs),$(wildcard $(dir)/*.o))     

# shell command to execute make in all directories
#DO_MAKE = @ for i in $(dirs); do $(MAKE) -C $$i $@; done
DO_MAKE = @if test -e $(LIBS) ; then for i in $(dirs); \
             do $(MAKE) -C $$i $@; done ; \
          else  \
             echo "asmlib.a needed for compile" ; \
          fi

# template for each source compile
%.o:    %.asm $(incs)
	$(CC) $(DEBUG) $(CFLAGS) $<

# template for link
%:      %.o
	ld $^ $(LFLAGS) -o $@ $(LIBS)


all:  pre $(dirs) post
	$(DO_MAKE)

pre:
	touch	*.asm
#	touch aeditSetup/*.asm
#	touch ShowSysErr/*.asm
	strip file_browse

post: aedit


doc:
	../txt2man -t aedit aedit.txt | gzip -c > aedit.1.gz 

#
# the "install" program uses flags
#        -D       create any needed directories
#        -s       strip executables
#        -m 644   set file attributes to 644 octal
install:
	@if test -w /etc/passwd ; \
	then \
	 echo "installing aedit in /usr/bin" ; \
	 install -s -m 777 aedit /usr/bin ; \
	  if test -e /usr/bin/a ; \
	  then \
	  echo "no shortcut (a) to aedit in /usr/bin" ; \
	  else \
	  install -m 766 --owner=$(usr) a /usr/bin ; \
	  fi \
	else \
	  echo "-" ; \
	  echo "Root access needed to install at /usr/bin and /usr/share/aedit" ; \
	  echo "aborting install, switch to root user with su or sudo then retry" ; \
	fi

uninstall:
	@if test -w /etc/passwd ; \
	then \
	 echo "uninstalling aedit from /usr/bin" ; \
	 rm -f /usr/bin/aedit ; \
	  echo "-" ; \
	  echo "Root access needed to uninstall /usr/share/aedit" ; \
	  echo "aborting uninstall, switch to root user with su or sudo then retry" ; \
	  fi
	@if test -d $(HOME)/bin ; \
	then \
	echo "removing shortcut (a) to aedit in $(HOME)/bin" ; \
	rm -f $(HOME)/bin/a ; \
	fi \

release: tar

tar:
	strip aedit
	if [ ! -e "../release" ] ; then mkdir ../release ; fi
	tar cfz ../release/aedit.tar.gz -C .. aedit


