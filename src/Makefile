# Makefile for src directory

include ../Makefile.common

sources = \
    RWLock.C \

# Flags needed to link
ADDITIONAL_LDFLAGS += \


objects = $(sources:.C=.o)

libs = \
    lib$(library).$(DYNAMIC_EXT) \
    lib$(library).a \

LDFLAGS += \
    $(ADDITIONAL_LDFLAGS) \


.PHONY: all
all: $(libs)

$(libs): $(objects)

.PHONY: clean
clean:
	@echo Making `pwd` $@
	@rm -f *.o $(libs)
	@$(MAKE) -C test $@

.PHONY: distclean
distclean: clean
	@echo Making `pwd` $@
	@rm -rf html
	@rm -f *.d
	@rm -f $(addprefix $(LIB_STATIC_DIR)/, $(libs))
	@rm -f $(addprefix $(LIB_DIR)/, $(libs))
	@$(MAKE) -C test $@

.PHONY: test check
test check: all
	@echo Making `pwd` $@
	@$(MAKE) -C test $@

# this target is experimental; gcc and clang differ; ignore
.PHONY: debug
debug:
	@echo Making `pwd` $@
	@$(MAKE) CDEBUG="-g" CXXOPT="-O0"


# Include this to generate make dependencies for C '.h' files. Don't
# evaluate dependencies for 'clean' so that those
# targets will still work when '.d' files contain references to
# recently-deleted '.h' files.
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
    -include $(sources:.C=.d)
endif
