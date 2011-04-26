GTEST_LIB_PATH = $(LIB_PATH)/gtest
GTEST_LIB = $(GTEST_LIB_PATH)/libgtest.a
GTEST_HEADER_PATH = $(GTEST_LIB_PATH)/include

CFLAGS += -I$(GTEST_HEADER_PATH) -I$(PROJ_ROOT)/include

TEST_SOURCE_FILES = testDate.cc testInstrument.cc testYieldCurve.cc testUtility.cc testMain.cc
TEST_OBJECT_FILES = $(patsubst %.cc, %, $(TEST_SOURCE_FILES))

DEP_LIBS = $(SOURCE_PATH)/core/YieldCurve.a $(SOURCE_PATH)/core/Tools.a\
		   $(GTEST_LIB) $(LIB_PATH)/boost/libboost_regex.a\
		   $(LIB_PATH)/boost/libboost_date_time.a

TARGET_PROG = verifyEngine

.PHONY: all clean run

all: $(TARGET_PROG)

$(TARGET_PROG): $(DEP_LIBS) $(TEST_OBJECT_FILES)
	$(CXX) $(CFLAGS) -o $(TARGET_PROG) $^


$(TEST_OBJECT_FILES): %:%.cc $(DEP_LIBS) 
	$(CXX) $(CFLAGS) -c -o $@ $<

run: $(TARGET_PROG)
	./$(TARGET_PROG)

clean:
	$(RM) $(TEST_OBJECT_FILES)
	$(RM) $(TARGET_PROG)
