CFLAGS += -I$(PROJ_ROOT)/include 
#CFLAGS += -isystem/opt/local/include

SOURCE_FILES = generateYieldCurve.cc optionMCSim.cc
EXEC_FILES = $(patsubst %.cc, %, $(SOURCE_FILES))

DEP_LIBS = $(PROJ_ROOT)/src/core/YieldCurve.a\
		   $(PROJ_ROOT)/src/core/Tools.a\
		   $(LIB_PATH)/boost/libboost_regex.a\
		   $(LIB_PATH)/boost/libboost_date_time.a
		   #$(PROJ_ROOT)/src/core/Stock.a\

.PHONY: all clean

all: $(BIN_PATH) $(EXEC_FILES)

$(BIN_PATH):
	if [ ! -d $(BIN_PATH) ]; then\
		mkdir $(BIN_PATH); \
	fi

$(EXEC_FILES): %:%.cc
	$(CXX) $(CFLAGS) -o $(BIN_PATH)/$@ $< $(DEP_LIBS)

clean:
	$(RM) -r $(BIN_PATH)
