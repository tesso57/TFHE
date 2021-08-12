CC := g++-10
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic
INC= -I ./include/
SRCROOT = .
SRCDIRS := $(shell find $(SRCROOT) -type d)
SRCS=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJS:=$(SRCS:.cpp=.o)
.PHONY: all
all : main clean

main : $(OBJS)
	$(CC) -o $@ $^

.cpp.o:
	$(CC) $(CXXFLAGS) -c $< $(INC) -o $(<:.cpp=.o)

.PHONY: clean
clean :
	rm $(OBJS)