CC 					 := gcc
CXX					 := g++
ASM		    		 := nasm

CSTD        		 := 99
DEBUG                := 0
override CFLAGS      := -Wall -Wextra -std=gnu$(CSTD) -Iinclude $(CFLAGS)
override CFLAGS      += -Wall -Werror -Wextra -Wparentheses -Wmissing-declarations -Wunreachable-code -Wunused 
override CFLAGS		 += -Wmissing-field-initializers -Wmissing-prototypes -Wswitch-enum
override CFLAGS	 	 += -Wredundant-decls -Wshadow -Wswitch-default -Wuninitialized
override CFLAGS      += -fstrength-reduce -fomit-frame-pointer -finline-functions 
override CXXFLAGS    := -Wall -Wextra -Wpedantic $(CXXFLAGS)
override ASMFLAGS    := -felf64 -g $(ASMFLAGS)

DUMMY       		 := examples/test.c
DUMMYOBJ    	 	 := $(patsubst examples/%,object/%.o, $(basename $(DUMMY)))
DEPS        		 := $(wildcard object/*.d)
SRCDIR               := src
OBJDIR               := object 
INCDIR               := include
BINDIR               := bin
CFILES   		     := $(wildcard src/*.c) 
CXXFILES  		     := $(wildcard src/*.cpp)
ASMFILES  		     := $(wildcard src/*.S) 
OBJECTS  		     := $(patsubst src/%,object/%.o, $(basename $(CFILES) $(CXXFILES) $(ASMFILES))) 

BIN		             := $(BINDIR)/out

ifeq ($(DEBUG), 1)
override CFLAGS+=-ggdb -O0
else 
override CFLAGS+=-O2
endif

ifeq ($(SANITIZE),1)
override CFLAGS+=-fsanitize=address -fsanitize=undefined
endif

.PHONY: all run debug clean

all : $(BIN)

deploy :
	@mkdir -p examples
	@mkdir -p $(SRCDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(BINDIR)

run : $(BIN)
	$(BIN)

dump : $(BIN)
	objdump -Mintel -d $(BIN) > dump.txt

debug : CFLAGS += -ggdb -O0
debug : $(BIN)
	gdb -q --args $(BIN) $(ARGS)

clean :
	rm -rf bin/*
	rm -rf object/*

$(BIN) : $(OBJECTS) $(DUMMYOBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(DUMMYOBJ) : 
ifeq ($(suffix $(DUMMY)), .c) 
	$(CC) -MMD $(CFLAGS) -c -o $@ $(DUMMY)
else ifeq ($(suffix $(DUMMY)), .cpp)
	$(CXX) -MMD $(CXXFLAGS) -c -o $@ $(DUMMY)
endif 

object/%.o : $(SRCDIR)/%.c $(OBJDIR)/
	$(CC) -MMD $(CFLAGS) -c -o $@ $<
object/%.o : $(SRCDIR)/%.S $(OBJDIR)/
	$(ASM) $(ASMFLAGS) -c -o $@ $< 
object/%.o : $(SRCDIR)/%.cpp $(OBJDIR)/
	$(CXX) -MMD $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/ :
	@mkdir $(OBJDIR)

$(BINDIR)/ :
	@mkdir $(BINDIR)

-include ($(DEPS))