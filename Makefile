CC = g++
CFLAGS = -std=c++14 -Wall -Iinclude/ -g
SRCS = $(shell find src/ -name "*.cc")
HEADERS = $(shell find include/ -name "*.hpp")
OBJS = $(patsubst %.cc, %.o, $(SRCS))
DYNAMIC_LIB = libcoroutine.so
TEST_SRC = test/test.cc
TEST_OBJ = test/test.o
TEST_TARGET = test/test

all: $(OBJS)

test: $(TEST_TARGET)
	@echo "开始执行测试程序"
	$(TEST_TARGET)

debug: $(TEST_TARGET)
	cgdb $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) $(TEST_SRC) -o $@

%.o: %.cc  $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(TEST_OBJ) $(OBJS) $(TEST_TARGET)
