SOURCE_PATH = $(CURDIR)/src
TEST_PATH = $(CURDIR)/test

export LIB_PATH = $(CURDIR)/lib
export CXX = llvm-g++

.PHONY: all test clean

all:
	$(MAKE) -C $(SOURCE_PATH)

test:
	$(MAKE) -C $(TEST_PATH)

clean:
	$(MAKE) -C $(SOURCE_PATH) clean
	$(MAKE) -C $(TEST_PATH) clean

