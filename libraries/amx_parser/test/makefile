ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS = amx_parser_store_check.o \
		  mock_malloc.o \
		  mock_amx_htable_insert.o \
		  mock_strdup.o

TARGET = amx_parser_store_check

CFLAGS  += -O2 -std=c99 -g -Wall -W -Werror -I. -I../include -I$(AMX_INCLUDE_DIR) \
		   $(shell pkg-config --cflags check amx_parser amx_containers)
LDFLAGS += -L$(AMX_LIB_DIR) \
		   $(shell pkg-config --libs check amx_parser amx_containers) -ldl -lpthread

ifdef CONFIG_GLIBC_MALLOC_BUILD
LIBS+=$(AMX_OUTPUT_DIR)/lib/libglibcmalloc.a
CFLAGS+=-DMOCK_MALLOC
endif

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

all: $(TARGET)
	LD_LIBRARY_PATH=$(AMX_LIB_DIR):$(CHECK_LIB_DIR):../source/ ./amx_parser_store_check
ifdef AMX_TEST_COVERAGE
	$(LCOV) --base-directory ../source --directory ../source --capture --output-file amx_parser_store.info
	$(GENHTML) -o ./coverage amx_parser_store.info
	$(INSTALL) -D -p -m 0664 amx_parser_store.info $(AMX_OUTPUT_DIR)/coverage/amx_parser_store.info
ifdef AMX_TEST_COVERAGE_SHOW
	$(BROWSER) ./coverage/index.html
endif
else
	@echo "AMX_TEST_COVERAGE is not set"
endif

$(TARGET): $(OBJECTS)
	$(CC) -o $(@) $(OBJECTS) $(LIBS) $(LDFLAGS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(OBJECTS) $(TARGET) *.d *.gcno *.gcov *.gcda amx_parser_store.info
	rm -rf ./coverage

.PHONY: clean
