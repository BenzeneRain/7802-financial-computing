GTEST_LIB_PATH = $(LIB_PATH)/gtest
GTEST_HEADER_PATH = $(GTEST_LIB_PATH)/include

TEST_SOURCE_FILES = testDate.cc testInstrument.cc testYieldCurve.cc
TEST_OBJECT_FILES = $(patsubst %.cc, %.o, $(TEST_SOURCE_FILES))

.PHONY: all clean

all: $(TEST_OBJECT_FILES)

$(TEST_OBJECT_FILES): %.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $< 

clean:
	$(RM) $(TEST_OBJECT_FILES)
