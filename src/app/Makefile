CFLAGS += -I$(PROJ_ROOT)/include 
#CFLAGS += -isystem/opt/local/include

SOURCE_FILES = generateYieldCurve.cc optionMCSim.cc
OUTPUT_DIR = $(PROJ_ROOT)/bin
EXEC_FILES = $(patsubst %.cc, %, $(SOURCE_FILES))

DEP_LIBS = $(PROJ_ROOT)/src/core/YieldCurve.a\
		   $(PROJ_ROOT)/src/core/Tools.a\
		   $(PROJ_ROOT)/src/core/Stock.a\
		   $(LIB_PATH)/boost/libboost_regex.a\
		   $(LIB_PATH)/boost/libboost_date_time.a

.PHONY: all clean

all: $(OUTPUT_DIR) $(EXEC_FILES)

$(OUTPUT_DIR):
	if [ ! -d $(OUTPUT_DIR) ]; then\
		mkdir $(OUTPUT_DIR); \
	fi

$(EXEC_FILES): %:%.cc
	$(CXX) $(CFLAGS) -o $(OUTPUT_DIR)/$@ $< $(DEP_LIBS)

clean:
	$(RM) -r $(OUTPUT_DIR)
