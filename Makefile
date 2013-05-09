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
TARGETDEFINE=-DWINDOWS
OSNAME=windows
OBJS:=$(OBJS) windowsResource.o
CPPFLAGS:=$(CPPFLAGS) -mwindows
LDFLAGS:=$(LDFLAGS) -mwindows
windowsResource.o: windowsResource.rc bcj.ico
	windres $< -o $@
endif
ifeq ($(UNAME), Linux )
TARGET=lnz
TARGETDEFINE=-DANDROID
OSNAME=android
endif

# Actual build rules.
SRCS:=$(SRCS) $(wildcard ./*.hpp) $(wildcard ./*/*.hpp) $(wildcard ./*.cpp)
CPPS:=$(CPPS) $(wildcard ./*.cpp)
OBJS:=$(OBJS) $(CPPS:.cpp=.o)
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
release: CPPFLAGS:=-O4 $(TARGETDEFINE) -flto $(CPPFLAGS)
release: LDFLAGS:=-O4 -flto $(LDFLAGS)
release: STRIP:=strip -p $(TARGET)
endif
ifeq ($(OSNAME), android)
release: CPPFLAGS:=-O4 $(TARGETDEFINE) $(CPPFLAGS)
release: LDFLAGS:=-O4 $(LDFLAGS)
release: STRIP:=strip -p $(TARGET)
endif

.PHONY: debug 
debug: $(TARGET)
debug: CPPFLAGS:=$(TARGETDEFINE) -DDEBUG $(CPPFLAGS)


.PHONY: clean
clean:
	rm -f ./*.o $(TARGET)

.PHONY: backup
backup:
	make -C ../ backup

.PHONY: depend
depend:
	gcc -std=c++11 $(TARGETDEFINE) -MM $(CPPS) > ./deps

.PHONY: run
run: all
	./$(TARGET)
