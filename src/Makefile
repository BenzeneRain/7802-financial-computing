CORE_PATH = $(CURDIR)/core
APP_PATH = $(CURDIR)/app

.PHONY: all app core clean

all: core app

core:
	$(MAKE) -C $(CORE_PATH)

app:
	$(MAKE) -C $(APP_PATH)

clean:
	$(MAKE) -C $(CORE_PATH) clean
	$(MAKE) -C $(APP_PATH) clean 
