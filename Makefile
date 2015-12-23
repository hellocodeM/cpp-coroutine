CC = g++
CFLAGS = -std=c++14 -Wall -Iinclude/ -g
SRCS = $(shell find src/ -name "*.cc")
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TEST_SRC = test/test.cc
TEST_OBJ = test/test

test: $(TEST_SRC) $(SRCS) 
	$(CC) $(CFLAGS) $(TEST_SRC) $(SRCS) -o $(TEST_OBJ)
	$(TEST_OBJ)

debug: test $(TEST_OBJ)
	cgdb $(TEST_OBJ)

clean:
	rm $(TARGET) $(TEST_OBJ)
