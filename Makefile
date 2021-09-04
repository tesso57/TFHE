CC := g++-10
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic
INC= -I ./include/
SRCROOT:= ./src
SRCDIRS := $(shell find $(SRCROOT) -type d)
SRCS:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJS:=$(SRCS:.cpp=.o)
OBJSDIR:=./obj
TESTOBJS:=$(filter-out ./src/main.o,$(OBJS))
.PHONY: all
all : test main

main : $(OBJS)
	$(CC) -o $@.out $(addprefix $(OBJSDIR)/,$(notdir $^))

.cpp.o:
	$(CC) $(CXXFLAGS) -c $< $(INC) -o $(addprefix $(OBJSDIR)/,$(notdir $(<:.cpp=.o)))

test_tlwe : $(TESTOBJS) ./test/test_tlwe.o
	$(CC) -o test_tlwe.out  $(addprefix $(OBJSDIR)/,$(notdir $^))

test_trlwe : $(TESTOBJS) ./test/test_trlwe.o
	$(CC) -o test_trlwe.out  $(addprefix $(OBJSDIR)/,$(notdir $^))

test_trgsw : $(TESTOBJS) ./test/test_trgsw.o
	$(CC) -o test_trgsw.out  $(addprefix $(OBJSDIR)/,$(notdir $^))
.PHONY: test
test : test_tlwe test_trlwe test_trgsw