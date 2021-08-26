CC := g++-10
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic
INC= -I ./include/
SRCROOT:= ./src
SRCDIRS := $(shell find $(SRCROOT) -type d)
SRCS:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJS:=$(SRCS:.cpp=.o)
TESTOBJS:=$(filter-out ./src/main.o,$(OBJS))
TEST=test_tlwe
.PHONY: all
all : test main clean

main : $(OBJS)
	$(CC) -o $@.out $^

.cpp.o:
	$(CC) $(CXXFLAGS) -c $< $(INC) -o $(<:.cpp=.o)

test : $(TESTOBJS) ./test/$(TEST).o
	$(CC) -o $(TEST).out  $^
	rm $^

.PHONY: clean
clean :
	rm $(OBJS)