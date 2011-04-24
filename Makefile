export TEST_PATH = $(CURDIR)/test
export PROJ_ROOT = $(CURDIR)
export SOURCE_PATH = $(CURDIR)/src
export LIB_PATH = $(CURDIR)/lib
export CXX = llvm-g++
#export CXX = g++

#CFLAGS = -O0 -ggdb3
CFLAGS  = -O2
CFLAGS += -isystem/opt/local/include
export CFLAGS

.PHONY: all test run-test clean

all: source test

source:
	$(MAKE) -C $(SOURCE_PATH)

test:
	$(MAKE) -C $(TEST_PATH)

run-test:
	$(MAKE) -C $(TEST_PATH) run

clean:
	$(MAKE) -C $(SOURCE_PATH) clean
	$(MAKE) -C $(TEST_PATH) clean

