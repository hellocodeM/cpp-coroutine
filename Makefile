CC = g++
CFLAGS = -std=c++14 -Wall -I include/ -fPIC -shared
SRCS = $(shell find src/ -name "*.cc")
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = libcoroutine.so
TEST_SRC = test/test.cc
TEST_OBJ = test/test

all: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

test: $(TEST_SRC) $(SRCS) all
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -lcoroutine -o $(TEST_OBJ)
	$(TEST_OBJ)

clean:
	rm $(TARGET) $(TEST_OBJ)
