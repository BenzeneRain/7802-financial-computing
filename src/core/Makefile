CFLAGS += -c 


YIELDCURVE_SOURCE_FILES = Instrument.cc YieldCurve.cc
TOOLS_SOURCE_FILES = Date.cc Utility.cc
STOCK_SOURCE_FILES = Stock.cc

YIELDCURVE_OBJECT_FILES = $(patsubst %.cc, %.o, $(YIELDCURVE_SOURCE_FILES))
TOOLS_OBJECT_FILES = $(patsubst %.cc, %.o, $(TOOLS_SOURCE_FILES))
STOCK_OBJECT_FILES = $(patsubst %.cc, %.o, $(STOCK_SOURCE_FILES))
OBJECT_FILES = $(YIELDCURVE_OBJECT_FILES) $(TOOLS_OBJECT_FILES) $(STOCK_OBJECT_FILES)

AR_FILES = YieldCurve.a Tools.a Stock.a

.PHONY: all clean

all: $(OBJECT_FILES) $(AR_FILES)

$(OBJECT_FILES): %.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $<

YieldCurve.a: $(YIELDCURVE_OBJECT_FILES)
	ar -rv $@ $^

Tools.a:$(TOOLS_OBJECT_FILES)
	ar -rv $@ $^

Stock.a:$(STOCK_OBJECT_FILES) 
	ar -rv $@ $^


clean:
	$(RM) $(OBJECT_FILES)
	$(RM) $(AR_FILES)
