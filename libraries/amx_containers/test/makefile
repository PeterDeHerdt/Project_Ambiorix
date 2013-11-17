ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS =	amx_containers_check.o \
			amx_llist_check.o \
			amx_llist_it_check.o \
			amx_lstack_check.o \
			amx_lqueue_check.o \
			amx_array_check.o

TARGET = amx_containers_check

CFLAGS  += -std=c99 -g -Wall -W -Werror -fPIC -I. -I../include -I$(AMX_INCLUDE_DIR) \
			$(shell pkg-config --cflags check amx_containers)
LDFLAGS += -L$(AMX_LIB_DIR) \
			$(shell pkg-config --libs check amx_containers)

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

ifdef AMX_BUILD_OPTIMIZE
CFLAGS += -O2
endif

all: $(TARGET)
	LD_LIBRARY_PATH=$(CHECK_LIB_DIR):../source/ ./amx_containers_check
ifdef AMX_TEST_COVERAGE
	$(LCOV) --base-directory ../source --directory ../source --capture --output-file amx_containers.info
	$(GENHTML) -o ./coverage amx_containers.info
	$(INSTALL) -D -p -m 0664 amx_containers.info $(AMX_OUTPUT_DIR)/coverage/amx_containers.info
ifdef AMX_TEST_COVERAGE_SHOW
	$(BROWSER) ./coverage/index.html
endif
else
	@echo "AMX_TEST_COVERAGE is not set"
endif

$(TARGET): $(OBJECTS)
	$(CC) -fPIC $(LDFLAGS) -o $(@) $(OBJECTS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(OBJECTS) $(TARGET) *.d *.gcno *.gcov *.gcda amx_containers.info
	rm -rf ./coverage

.PHONY: clean
