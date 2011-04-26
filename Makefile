export TEST_PATH = $(CURDIR)/test
export PROJ_ROOT = $(CURDIR)
export SOURCE_PATH = $(CURDIR)/src
export LIB_PATH = $(CURDIR)/lib
export BIN_PATH = $(CURDIR)/bin
export DATA_PATH = $(CURDIR)/data

export CXX = llvm-g++
#export CXX = g++

#CFLAGS = -O0 -ggdb3
CFLAGS  = -O2
CFLAGS += -isystem/opt/local/include
export CFLAGS

.PHONY: all test run-test clean\
	run run-part1 run-part2

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

run: run-part1 run-part2

run-part1:
	@echo "\nRun Project Part 1, the results are written to out.csv"
	$(BIN_PATH)/generateYieldCurve $(DATA_PATH)/curveSpec1.csv $(DATA_PATH)/curveDataInput1.csv $(DATA_PATH)/queryInput.csv out.csv

run-part2:
	@echo "\nRun Project Part 2"
	$(BIN_PATH)/optionMCSim $(DATA_PATH)/curveSpec1.csv $(DATA_PATH)/curveDataInput1.csv $(DATA_PATH)/optionDesc.csv 
