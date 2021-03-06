# Makefile for unit tests

include ../../Makefile.common

sources = \
    MockWithTemplatesTest.C \
    RWLockTest.C \

# Flags needed to link
ADDITIONAL_LDFLAGS += \


objects = $(sources:.C=.o)

target = alltests

# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += \
    -isystem $(GTEST_INCLUDE_DIR) \
    -isystem $(GMOCK_INCLUDE_DIR) \
    -I.. \

CXXFLAGS += \

LDFLAGS += \
    -L$(GTEST_LIB_DIR) \
    -L$(LIB_STATIC_DIR) \
    $(ADDITIONAL_LDFLAGS) \
    -l$(library) \
    -lgtest \
    -lgmock \
    -lgmock_main \

.PHONY: all
all: $(target)

$(target): $(objects) 
	$(CXX) $(LDFLAGS) $(objects) -o $@

.PHONY: clean
clean:
	@echo Making `pwd` $@
	@rm -f *.o $(target)

.PHONY: distclean
distclean: clean
	@echo Making `pwd` $@
	@rm -rf html
	@rm -f *.d


# Setup DYLD_LIBRARY_PATH
ld_library_path := $(LIB_DIR)
ifneq ($(DYLD_LIBRARY_PATH),)
    ld_library_path := $(ld_library_path):$(DYLD_LIBRARY_PATH)
endif
export DYLD_LIBRARY_PATH := $(ld_library_path)


.PHONY: test check
test check: $(target)
	@echo Making `pwd` $@
	@echo
	@echo DYLD_LIBRARY_PATH=$(DYLD_LIBRARY_PATH)
	@echo
ifneq ($(OPTIONS),)
	./$(target) $(OPTIONS)
else
	./$(target)
endif

# Include this to generate make dependencies for C '.h' files. Don't
# evaluate dependencies for 'clean' so that those
# targets will still work when '.d' files contain references to
# recently-deleted '.h' files.
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
    -include $(sources:.C=.d)
endif

