CFLAGS += -I./include -isystem/opt/local/include -c -ggdb3 -O0

SOURCE_FILES = Date.cc Instrument.cc YieldCurve.cc Utility.cc
OBJECT_FILES = $(patsubst %.cc, %.o, $(SOURCE_FILES))

AR_FILES = YieldCurve.a

.PHONY: all clean

all: $(OBJECT_FILES) $(AR_FILES)

$(OBJECT_FILES): %.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $<

$(AR_FILES): $(OBJECT_FILES)
	ar -rv $@ $(OBJECT_FILES)

clean:
	$(RM) $(OBJECT_FILES)
	$(RM) $(AR_FILES)
