################################################################################
## Copyright (c) Jonathan(Jon) DuBois 2013. This file is part of LNZ.         ##
##                                                                            ##
## LNZ is free software: you can redistribute it and/or modify it under the   ##
## terms of the GNU General Public License as published by the Free Software  ##
## Foundation, either version 3 of the License, or (at your option) any later ##
## version.                                                                   ##
##                                                                            ##
## LNZ is distributed in the hope that it will be useful, but WITHOUT ANY     ##
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  ##
## FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more     ##
## details.                                                                   ##
##                                                                            ##
## You should have received a copy of the GNU General Public License along    ##
## with LNZ, located in the file GPL.txt.  If not, see                        ##
## <http://www.gnu.org/licenses/>.                                            ##
##                                                                            ##
## Alternative licensing terms may be available for a fee, contact Jon DuBois ##
## (bcj1980@gmail.com) for details.                                           ##
################################################################################

# TODO: Write gcc and 32-bit portions.



all: debug lnz.exe



# Toolchain.
CPP=g++
CPPFLAGS=-std=c++11 -Wall -fexceptions -pedantic -Wextra -Werror -c
LD=g++
LDFLAGS=


# Actual build rules.
SRCS=lnz.hpp os.hpp parse.hpp strings.hpp main.cpp os.cpp parse.cpp
OBJS=main.o os.o parse.o
$(OBJS): lnz.hpp strings.hpp Makefile
os.o: os.hpp
parse.o: parse.hpp
main.o: os.hpp parse.hpp

# Override defaults
%.o: %.cpp
	$(CPP) $(CPPSYSFLAGS) $(CPPFLAGS) $< -o $@

lnz.exe: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(STRIP)


UNAME=$(shell uname)
.PHONY: release 
release:
ifeq ($(UNAME), MINGW32_NT-6.1)
	$(eval CPPFLAGS:=-O4 -DWINDOWS -flto $(CPPFLAGS))
	$(eval LDFLAGS:=-O4 -flto $(LDFLAGS))
	$(eval STRIP:=strip -p lnz.exe)
endif
ifeq ($(UNAME), Linux)
	$(eval CPPFLAGS:=-O4 -DANDROID $(CPPFLAGS))
	$(eval LDFLAGS:=-O4 $(LDFLAGS))
	$(eval STRIP:=strip -p lnz.exe)
endif

.PHONY: debug
debug:
ifeq ($(UNAME), MINGW32_NT-6.1)
	$(eval CPPFLAGS:=-DWINDOWS -DDEBUG $(CPPFLAGS))
endif
ifeq ($(UNAME), Linux)
	$(eval CPPFLAGS:=-DANDROID -DDEBUG $(CPPFLAGS))
endif


.PHONY: clean
clean:
	rm -f ./*.o ./*.exe

.PHONY: backup
backup:
	make -C ../ backup

TAGS: $(SRCS)
	etags --declarations --ignore-indentation $^
