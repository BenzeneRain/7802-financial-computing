GTEST_LIB_PATH = $(LIB_PATH)/gtest
GTEST_LIB = $(GTEST_LIB_PATH)/libgtest.a
GTEST_HEADER_PATH = $(GTEST_LIB_PATH)/include

CFLAGS += -I$(GTEST_HEADER_PATH) -I$(PROJ_ROOT)/include -isystem/opt/local/include  -ggdb3 -O0

TEST_SOURCE_FILES = testDate.cc testInstrument.cc testYieldCurve.cc
TEST_OBJECT_FILES = $(patsubst %.cc, %, $(TEST_SOURCE_FILES))

DEP_LIBS = $(SOURCE_PATH)/YieldCurve.a $(GTEST_LIB) $(LIB_PATH)/boost/libboost_regex.a

.PHONY: all clean run

all: $(DEP_LIBS) $(TEST_OBJECT_FILES)

$(TEST_OBJECT_FILES): %:%.cc $(DEP_LIBS) 
	$(CXX) $(CFLAGS) -o $@ $< $(DEP_LIBS)

run: 
	for testprog in $(TEST_OBJECT_FILES); do sh -c $(CURDIR)/$$testprog; done

clean:
	$(RM) $(TEST_OBJECT_FILES)