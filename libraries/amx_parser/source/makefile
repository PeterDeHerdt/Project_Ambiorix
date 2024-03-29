ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS = amx_parser_store.o

TARGET = libamx_parser_store

CFLAGS  += -std=c99 -g -Wall -W -fPIC -I. -I../include -I$(AMX_INCLUDE_DIR)
LDFLAGS += -L$(AMX_LIB_DIR) $(shell pkg-config --libs amx_containers)

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

ifdef AMX_BUILD_OPTIMIZE
CFLAGS += $(AMX_BUILD_OPTIMIZE)
endif

all: $(TARGET).so $(TARGET).a

$(TARGET).so: $(OBJECTS)
	$(CC) -Wl,-soname,$(@) -shared -fPIC $(LDFLAGS) -o $(@) $(OBJECTS)

$(TARGET).a: $(OBJECTS)
	$(AR) rcs $(@) $(OBJECTS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(OBJECTS) $(TARGET).so $(TARGET).a *.d *.gcno *.gcov *.gcda

.PHONY: clean coverage
