CFLAGS += -I./include -c 
#CFLAGS += -isystem/opt/local/include
#CFLAGS += -ggdb3 -O0
CFLAGS += -O2


SOURCE_FILES = Date.cc Instrument.cc YieldCurve.cc Utility.cc
OBJECT_FILES = $(patsubst %.cc, %.o, $(SOURCE_FILES))
DEP_LIBS = $(LIB_PATH)/boost/libboost_regex.a $(LIB_PATH)/boost/libboost_date_time.a

AR_FILES = YieldCurve.a

.PHONY: all clean

all: $(OBJECT_FILES) $(AR_FILES)

$(OBJECT_FILES): %.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $<

$(AR_FILES): $(OBJECT_FILES) $(DEP_LIBS)
	ar -rv $@ $(OBJECT_FILES) $(DEP_LIBS)

clean:
	$(RM) $(OBJECT_FILES)
	$(RM) $(AR_FILES)
