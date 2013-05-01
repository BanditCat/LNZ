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
## with LNZ, located in the file LICENSE.  If not, see                        ##
## <http://www.gnu.org/licenses/>.                                            ##
##                                                                            ##
## Alternative licensing terms may be available for a fee, contact Jon DuBois ##
## (bcj1980@gmail.com) for details.                                           ##
################################################################################



.PHONY: all
all: debug



# Toolchain.
CPP=g++
CPPFLAGS=-std=c++11 -Wall -fexceptions -pedantic -Wextra -Werror -c
LD=g++
LDFLAGS=

# OS Detection.
UNAME=$(shell uname) 
ifeq ($(UNAME), MINGW32_NT-6.1 )
TARGET=lnz.exe
OSNAME=windows
endif
ifeq ($(UNAME), Linux )
TARGET=lnz
OSNAME=android
endif

# Actual build rules.
SRCS=$(wildcard ./*.hpp) $(wildcard ./*.cpp)
CPPS=$(wildcard ./*.cpp)
OBJS=$(CPPS:.cpp=.o)
$(OBJS): Makefile
include deps

# Override defaults
%.o: %.cpp
	$(CPP) $(CPPSYSFLAGS) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(STRIP)

TAGS: $(SRCS)
	etags --declarations --ignore-indentation $^


.PHONY: release 
release: $(TARGET)
ifeq ($(OSNAME), windows)
release: CPPFLAGS:=-O4 -DWINDOWS -flto $(CPPFLAGS)
release: LDFLAGS:=-O4 -flto $(LDFLAGS)
release: STRIP:=strip -p $(TARGET)
endif
ifeq ($(OSNAME), android)
release: CPPFLAGS:=-O4 -DANDROID $(CPPFLAGS)
release: LDFLAGS:=-O4 $(LDFLAGS)
release: STRIP:=strip -p $(TARGET)
endif

.PHONY: debug
debug: $(TARGET)
ifeq ($(OSNAME), windows)
debug: CPPFLAGS:=-DWINDOWS -DDEBUG $(CPPFLAGS)
endif
ifeq ($(OSNAME), android)
debug: CPPFLAGS:=-DANDROID -DDEBUG $(CPPFLAGS)
endif


.PHONY: clean
clean:
	rm -f ./*.o $(TARGET)

.PHONY: backup
backup:
	make -C ../ backup

.PHONY: depend
depend:
	gcc -std=c++11 -MM $(CPPS) > ./deps
