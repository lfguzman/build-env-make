# Makefile for the library root directory

include Makefile.common

subdirs = \
    src \

.PHONY: all
all: $(subdirs)

.PHONY: $(subdirs)
$(subdirs): 
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: clean
clean: $(subdirs)

.PHONY: distclean
distclean: $(subdirs)

.PHONY: test check
test check: $(subdirs)
