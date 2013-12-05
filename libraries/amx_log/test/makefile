ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS = amx_log_check.o\
          mock_malloc.o

TARGET = amx_log_check

CFLAGS  += -O2 -std=c99 -g -Wall -W -Werror -I. -I../include -I$(AMX_INCLUDE_DIR) \
		   $(shell pkg-config --cflags check amx_log)
LDFLAGS += -L$(AMX_LIB_DIR) \
		   $(shell pkg-config --libs check amx_log) -ldl

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

all: $(TARGET)
	LD_LIBRARY_PATH=$(CHECK_LIB_DIR):../source/ ./amx_log_check
ifdef AMX_TEST_COVERAGE
	$(LCOV) --base-directory ../source --directory ../source --capture --output-file amx_log.info
	$(GENHTML) -o ./coverage amx_log.info
	$(INSTALL) -D -p -m 0664 amx_log.info $(AMX_OUTPUT_DIR)/coverage/amx_log.info
ifdef AMX_TEST_COVERAGE_SHOW
	$(BROWSER) ./coverage/index.html
endif
else
	@echo "AMX_TEST_COVERAGE is not set"
endif

$(TARGET): $(OBJECTS)
	$(CC) -o $(@) $(OBJECTS) $(AMX_OUTPUT_DIR)/lib/libglibcmalloc.a $(LDFLAGS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(OBJECTS) $(TARGET) *.d *.gcno *.gcov *.gcda amx_log.info
	rm -rf ./coverage

.PHONY: clean

