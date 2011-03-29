SOURCE_PATH = $(CURDIR)/src
TEST_PATH = $(CURDIR)/test

.PHONY: all test clean

all:
	$(MAKE) -C $(SOURCE_PATH)

clean:
	$(MAKE) -C $(SOURCE_PATH) clean

test:
	$(MAKE) -C $(TEST_PATH)

