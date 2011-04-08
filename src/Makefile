CFLAGS += -I./include -isystem/opt/local/include -c

SOURCE_FILES = Date.cc Instrument.cc YieldCurve.cc Utility.cc
OBJECT_FILES = $(patsubst %.cc, %.o, $(SOURCE_FILES))

.PHONY: all clean

all: $(OBJECT_FILES)

$(OBJECT_FILES): %.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJECT_FILES)
